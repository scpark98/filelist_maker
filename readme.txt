[파일 인코딩 관련]
- 프로젝트 설정에서 유니코드로 설정하고 UTF-8 포맷으로 filelist.lst를 생성하면
  Launcher.exe에서 InternetReadFile()로 읽어들일 때 잘못된 문자가 붙게된다.
  Launcher는 유니코드 설정으로 빌드되었으나 뭔가 읽어들이는 부분에서
  인코딩이 맞지 않는 부분이 있는듯하다.
  우선 멀티바이트로 설정하고 ANSI 포맷으로 filelist.lst를 생성하도록 한다.

  => 저장할 때 BOM을 날려주도록 아래와 같이 처리하여 해결함.
  //UTF-8로 저장하되 자동으로 붙는 BOM(3 char)를 날려준다.
  //그렇지 않으면 InternetReadFile()로 읽어올 때 BOM문자까지 읽어지므로 문제가 된다.
  fseek(fp, 0L, SEEK_SET);


[수정된 내용]
- exe를 그대로 download 할 경우 보안취약점 문제가 될 소지가 있어서
  파일목록 생성 시 각 파일들은 모두 각각 zip으로 압축한다.
  단, 파일타이틀이 동일한 경우도 있으므로 원래의 확장자를 그대로 유지시켜야 한다.
  (test.exe -> test.exe.zip)