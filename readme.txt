[파일 인코딩 관련]
- 프로젝트 설정에서 유니코드로 설정하고 UTF-8 포맷으로 filelist.txt를 생성하면
  Launcher.exe에서 InternetReadFile()로 읽어들일 때 잘못된 문자가 붙게된다.
  Launcher는 유니코드 설정으로 빌드되었으나 뭔가 읽어들이는 부분에서
  인코딩이 맞지 않는 부분이 있는듯하다.

- 우선 멀티바이트로 설정하고 ANSI 포맷으로 filelist.txt를 생성하도록 한다.
