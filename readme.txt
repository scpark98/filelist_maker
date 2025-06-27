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

[하위 폴더에 대한 규칙]
- 리스트 항목에 추가되는 대상 폴더가 존재할 경우 그 폴더내의 모든 파일/폴더를 리스트 항목에 추가하지 않고
  폴더명.zip이라는 하나의 압축파일로 추가함.
  (https://docs.google.com/presentation/d/1Rc18rcW05aMKB4FbQLFV-N-FKEg1hGmK62_01AzoLTQ/edit?slide=id.g361189aeeb7_0_16#slide=id.g361189aeeb7_0_16)
  따라서 filelist.lst로 생성하고자 하는 모든 파일+폴더들을 선택하여 drag&drop 해야 한다.
  생성하고자 하는 파일+폴더들의 parent 폴더를 drag&drop해서는 안된다.

[수정할 내용]
- drag&drop된 목록중에서 하위 폴더가 존재할 경우 해당 하위 폴더를 하나의 zip으로 압축한 뒤
  해당 폴더를 삭제시켜줘야 한다.


[수정된 내용]
*20240723
- 개별파일 추가 시 이미 존재하는 압축파일을 연 상태라면 zip 파일 생성이 실패하므로
  올바른 메시지 표시와 fclose()를 반드시 해줘야 한다.
  개별파일 추가일 경우는 filelist.lst 파일을 filelist.bak으로 복사하고
  filelist.lst 파일을 새로 생성하는데 어떤 파일의 압축이 실패했다면
  백업받은 flielist.bak을 filelist.lst로 복원시킨다.

- exe를 그대로 download 할 경우 보안취약점 문제가 될 소지가 있어서
  파일목록 생성 시 각 파일들은 모두 각각 zip으로 압축한다.
  단, 파일타이틀이 동일한 경우도 있으므로 원래의 확장자를 그대로 유지시켜야 한다.
  (test.exe -> test.exe.zip)