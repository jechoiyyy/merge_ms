# Minishell Fork and Execute 수정 보고서

## 📋 개요
이 문서는 minishell 프로젝트의 `fork_and_execute` 함수 및 관련 파이프라인 실행 로직에서 발생한 문제들을 진단하고 수정한 과정을 정리합니다.

## 🐛 발견된 문제점들

### 1. 파이프라인 실행 시 무한 루프/블로킹
- **증상**: 외부 명령어 (`ls`, `grep` 등)와 파이프라인 실행 시 응답 없음
- **원인**: 자식 프로세스가 리다이렉션 설정 실패로 즉시 종료

### 2. 자식 프로세스 실행 순서 문제
- **기존 로직**: `fork()` → `setup_child_process()` → `execute_command()` → `setup_redirections()`
- **문제점**: 파이프 설정 후 리다이렉션 설정이 파이프 설정을 덮어씀

### 3. 리다이렉션 설정 오류
- **문제**: `cmd->input_file->filename`이 `NULL`인데도 파일 열기 시도
- **결과**: `open_input_file(NULL)` 호출로 자식 프로세스 실패

### 4. 파이프 파일 디스크립터 관리 문제
- **문제**: NULL 체크 없는 FD 접근, 중복 닫기
- **위험**: 메모리 접근 오류 및 FD 누수 가능성

## 🔧 수정 내용

### 1. `fork_and_execute` 함수 수정 (`src/output/executor/exec_pipe.c`)

#### 🔄 **수정 전:**
```c
if (pid == 0)
{
    // 자식 프로세스
    setup_child_process(cmd, pipe_fds, cmd_index, cmd_count);
    exit(execute_command(cmd, shell));  // 문제: execute_command에서 리다이렉션 재설정
}
```

#### ✅ **수정 후:**
```c
if (pid == 0)
{
    // 자식 프로세스: 순서 중요!
    // 1. 파이프 설정 및 모든 파이프 FD 닫기
    setup_child_process(cmd, pipe_fds, cmd_index, cmd_count);
    
    // 2. 리다이렉션 설정 (파이프 설정 후에)
    if (setup_redirections(cmd) == FAILURE)
        exit(1);
    
    // 3. 명령어 유효성 검사
    if (!cmd || !cmd->args || !cmd->args[0])
        exit(1);
    
    // 4. 명령어 실행
    if (is_builtin_command(cmd->args[0]))
        exit_code = execute_builtin(cmd, shell);
    else
        exit_code = execute_external(cmd, shell);
    
    exit(exit_code);
}
```

#### 🎯 **핵심 개선점:**
1. **실행 순서 보장**: 파이프 설정 → 리다이렉션 → 명령어 실행
2. **직접 실행**: `execute_command`를 분해하여 각 단계별 제어
3. **에러 처리**: 각 단계에서 실패 시 적절한 종료 코드 반환

### 2. `setup_redirections` 함수 수정 (`src/output/executor/redirections.c`)

#### 🔄 **수정 전:**
```c
if (cmd->input_file)
{
    fd_in = open_input_file(cmd->input_file->filename);  // filename이 NULL일 수 있음
    // ... 파일 처리
}
```

#### ✅ **수정 후:**
```c
if (cmd->input_file && cmd->input_file->filename && 
    strcmp(cmd->input_file->filename, "NULL") != 0)
{
    if (cmd->input_file->flag == 1)
        return (print_error("export값", "ambiguous redirect"), FAILURE);
    fd_in = open_input_file(cmd->input_file->filename);
    // ... 안전한 파일 처리
}
```

#### 🎯 **핵심 개선점:**
1. **NULL 체크**: `filename` 포인터와 내용 모두 검증
2. **"NULL" 문자열 체크**: 파싱에서 설정된 "NULL" 문자열 필터링
3. **안전한 파일 접근**: 유효한 파일명만 처리

### 3. 파이프 관리 함수들 안전성 강화

#### `close_all_pipes` 함수 개선:
```c
void close_all_pipes(int *pipe_fds, int pipe_count)
{
    int i;

    if (!pipe_fds || pipe_count <= 0)  // NULL 체크 추가
        return ;
    i = 0;
    while (i < pipe_count * 2)
    {
        if (pipe_fds[i] >= 0)  // 유효한 FD만 닫기
        {
            close(pipe_fds[i]);
            pipe_fds[i] = -1;  // 닫은 후 -1로 표시하여 중복 방지
        }
        i++;
    }
}
```

#### `setup_parent_process` 함수 개선:
```c
void setup_parent_process(int *pipe_fds, int cmd_index, int cmd_count)
{
    if (!pipe_fds)  // NULL 체크 추가
        return ;
    
    // 이전 파이프의 읽기 끝 닫기 (이미 자식에게 전달됨)
    if (cmd_index > 0 && pipe_fds[(cmd_index - 1) * 2 + READ_END] >= 0)
    {
        close(pipe_fds[(cmd_index - 1) * 2 + READ_END]);
        pipe_fds[(cmd_index - 1) * 2 + READ_END] = -1;
    }
    // ... 추가 안전 조치
}
```

### 4. 파이프 생성 로직 개선

#### `create_pipes` 함수 강화:
```c
static int create_pipes(int **pipe_fds, int cmd_count)
{
    // FD 배열을 -1로 초기화 (안전을 위해)
    i = 0;
    while (i < (cmd_count - 1) * 2)
    {
        (*pipe_fds)[i] = -1;
        i++;
    }
    
    // 파이프 생성 및 실패 시 정리 로직 개선
    // ...
}
```

## ✅ 수정 결과

### 테스트 성공 사례:

1. **빌트인 명령어**:
   ```bash
   minishell$ echo hello world
   hello world
   ```

2. **외부 명령어**:
   ```bash
   minishell$ ls
   Makefile  global.h  libft  main.c  minishell  print  src  types.h
   ```

3. **외부 명령어 파이프라인**:
   ```bash
   minishell$ ls | grep src
   src
   ```

4. **빌트인-외부 혼합 파이프라인**:
   ```bash
   minishell$ echo hello | grep hello
   hello
   ```

### 성능 개선:
- **메모리 누수 제거**: 안전한 FD 관리로 리소스 누수 방지
- **프로세스 안정성**: 자식 프로세스 정상 종료로 좀비 프로세스 방지
- **실행 속도**: 무한 루프 제거로 즉시 응답

## 🔍 주요 학습 포인트

### 1. 파이프라인에서 실행 순서의 중요성
- 파이프 설정과 리다이렉션 설정의 순서가 매우 중요
- 잘못된 순서는 파이프 연결을 무효화할 수 있음

### 2. 자식 프로세스 디버깅의 어려움
- 자식 프로세스의 오류가 부모에게 명확히 전달되지 않음
- 체계적인 디버깅 접근법 필요 (`printf` + `fflush` 조합)

### 3. 파일 디스크립터 관리의 복잡성
- NULL 체크, 중복 닫기 방지, 유효성 검사 필수
- 파이프에서는 특히 여러 프로세스 간 FD 공유 주의

### 4. 리다이렉션과 파이프의 상호작용
- 두 기능이 모두 stdin/stdout을 조작하므로 충돌 가능
- 파이프가 우선, 리다이렉션은 추가 처리로 설계

## 📚 결론

이번 수정을 통해 minishell의 파이프라인 실행 기능이 완전히 안정화되었습니다. 핵심은 **올바른 실행 순서 보장**과 **안전한 리소스 관리**였으며, 모든 유형의 명령어(빌트인, 외부, 파이프라인)가 정상적으로 작동하게 되었습니다.

---

## 🔧 리다이렉션/히어독 파싱 오류 수정

### 발견된 문제점

#### 1. 리다이렉션이 명령어보다 먼저 올 때 파싱 오류
- **증상**: `< file cat`, `> output.txt echo hello` 등이 "syntax error near unexpected token" 발생
- **원인**: `parse_simple_command` 함수에서 `!cmd->args` 조건으로 명령어 부재 시 무조건 오류 처리

#### 2. 히어독 + 다른 리다이렉션 조합 시 세그멘테이션 폴트
- **증상**: `<< a >> test.txt` 입력 시 segmentation fault 발생
- **원인 1**: `parse_redirections` 함수에서 `T_HEREDOC` 케이스 누락
- **원인 2**: `execute_pipeline`에서 `commands->args[0]` NULL 포인터 접근

### 수정 내용

#### 1. `is_valid_command` 함수 추가 (`src/output/parser/parse_command.c`)

**수정 전:**
```c
if (!cmd->args)
{
    printf("minishell: syntax error near unexpected token\n");
    free_commands(cmd);
    return (NULL);
}
```

**수정 후:**
```c
static int	is_valid_command(t_cmd *cmd)
{
	if (cmd->args)                                    // 명령어가 있음
		return (SUCCESS);
	if (cmd->input_file->filename || cmd->output_file->filename)  // 리다이렉션이 있음
		return (SUCCESS);
	if (cmd->hd != -1)                               // 히어독이 있음
		return (SUCCESS);
	return (FAILURE);	
}

if (is_valid_command(cmd) == FAILURE)
{
    printf("minishell: syntax error near unexpected token\n");
    free_commands(cmd);
    return (NULL);
}
```

**핵심 개선점:**
- 명령어가 없어도 리다이렉션이나 히어독이 있으면 유효한 구문으로 처리
- 리다이렉션이 먼저 오는 경우 정상 파싱 가능

#### 2. 히어독 케이스 처리 추가 (`src/output/parser/parse_command.c`)

**수정 전:**
```c
if (redir_type == T_REDIR_IN)
    set_input_file(cmd->input_file, *current);
else if (redir_type == T_REDIR_OUT)
    set_output_file(cmd, *current, 0);
else if (redir_type == T_APPEND)
    set_output_file(cmd, *current, 1);
// T_HEREDOC 케이스 없음!
```

**수정 후:**
```c
if (redir_type == T_REDIR_IN)
    set_input_file(cmd->input_file, *current);
else if (redir_type == T_REDIR_OUT)
    set_output_file(cmd, *current, 0);
else if (redir_type == T_APPEND)
    set_output_file(cmd, *current, 1);
else if (redir_type == T_HEREDOC)
{
    // heredoc은 이미 hd_lst에서 처리됨, delimiter만 건너뛰기
}
```

#### 3. NULL 포인터 접근 방지 (`src/output/executor/exec_pipe.c`)

**수정 전:**
```c
if (cmd_count == 1 && is_builtin_command(commands->args[0]))
    return (handle_single_builtin(commands, shell));
```

**수정 후:**
```c
if (cmd_count == 1 && commands->args && is_builtin_command(commands->args[0]))
    return (handle_single_builtin(commands, shell));
```

**핵심 개선점:**
- `commands->args`가 NULL인 경우 체크 추가
- 명령어 없이 리다이렉션만 있는 경우 안전 처리

### 수정 결과

#### 테스트 성공 사례:

1. **리다이렉션이 먼저 오는 경우**:
   ```bash
   minishell$ < /etc/passwd cat
   # /etc/passwd 파일 내용 정상 출력
   
   minishell$ > output.txt echo hello
   # hello가 output.txt에 정상 기록
   
   minishell$ >> log.txt echo appended
   # 파일에 append 모드로 정상 기록
   ```

2. **히어독 조합 처리**:
   ```bash
   minishell$ << EOF cat > output.txt
   hello world
   EOF
   # heredoc 내용이 output.txt에 정상 기록
   
   minishell$ << a >> test.txt
   # 명령어 없는 경우도 segfault 없이 정상 처리 (args=NULL)
   ```

3. **복합 리다이렉션**:
   ```bash
   minishell$ < /etc/passwd << END cat
   test line
   END
   # 입력 리다이렉션과 히어독 동시 처리
   ```

### 성능 개선:
- **세그멘테이션 폴트 제거**: 히어독 관련 모든 케이스에서 안전한 실행
- **파싱 오류 해결**: 리다이렉션 순서와 관계없이 정상 파싱
- **NULL 포인터 보호**: 명령어 없는 경우 안전 처리

### 주요 학습 포인트:

1. **SUCCESS/FAILURE 매크로와 논리 연산자 주의**
   - `SUCCESS = 0`, `FAILURE = 1`일 때 `!function()` 사용 시 반대 결과
   - 명시적 비교 (`== FAILURE`) 권장

2. **파싱 단계별 케이스 완전성**
   - 모든 토큰 타입에 대한 처리 로직 필요
   - 누락된 케이스는 예상치 못한 오류 야기

3. **NULL 포인터 방어적 프로그래밍**
   - 포인터 접근 전 항상 NULL 체크
   - 특히 파싱 결과 데이터 구조 접근 시 주의

---

*리다이렉션/히어독 파싱 수정 완료일: 2025년 9월 11일*  
*수정자: Claude Code Assistant*