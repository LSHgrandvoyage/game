# Spaceship Game 🚀

**목소리로 비행기를 조종해 소행성을 피하는 게임.**
대학에서 처음 만든 프로젝트를 Processing(Java)에서 C++/SFML로 직접 마이그레이션하고, 지금도 개선하고 있습니다.

- 마이크에 소리를 내면 비행기가 상승하고, 조용하면 중력으로 떨어집니다
- 스페이스바로도 상승할 수 있습니다 (마이크 없이 플레이 가능)
- 소행성 기둥 사이를 통과하면 점수, 10점마다 레벨업 — 부딪히거나 추락하면 게임 오버, 클릭으로 재시작

📄 [게임 기획/소개 문서 (Notion)](https://ballistic-twig-9cd.notion.site/00-About-the-game-18fc4f296fb38037b119f9a6baff1873)

## 빌드 & 실행

macOS (Homebrew):

```bash
brew install sfml        # SFML 3.x
make
./game                   # 에셋이 상대경로라 저장소 루트에서 실행
```

- Makefile이 `brew --prefix`를 자동 감지합니다 (Linux에서는 시스템 SFML 사용)
- 첫 실행 시 마이크 권한 팝업이 뜹니다 — 거부해도 스페이스바로 플레이 가능
- 시작 후 1.5초는 주변 소음을 측정하는 보정 구간입니다 (화면에 `calibrating...` 표시). 조용히 기다리면 환경에 맞는 입력 기준이 자동 설정됩니다

## 아키텍처

```
src/
├── main.cpp                 진입점 — Game을 생성하고 run()
├── game/
│   └── Game                 게임 루프(입력→갱신→렌더), 상태·점수·충돌 판정 총괄
├── objects/
│   ├── Spaceship            플레이어 — 물리(중력·양력), 마이크/키 입력 반영
│   └── Obstacle             소행성 기둥 — 이동, 점수 판정, 난이도(간격) 조절
└── managers/
    ├── ResourceManager      텍스처·폰트 로드/보관 (싱글턴)
    └── AudioManager         BGM·효과음 + 마이크 입력 (싱글턴)
        └── MicVolumeRecorder  sf::SoundRecorder 상속 — 캡처 청크마다 RMS 계산
```

설계에서 신경 쓴 지점:

- **입력과 물리의 분리** — `Spaceship`은 "볼륨 값"만 받습니다. 마이크·노이즈 보정은 전부 `AudioManager` 안에 있어, 입력 소스를 바꿔도 물리 코드는 그대로입니다
- **프레임레이트 독립 물리** — 60fps 기준 상수를 `dt`로 정규화해, 어느 기기에서든 같은 속도로 동작합니다
- **마이크 노이즈 자동 보정** — 시작 1.5초간 주변 소음 평균을 측정해 노이즈 플로어를 잡고, 그 이상만 입력으로 인정합니다. 기기·환경마다 다른 소음 수준에 고정 임계값 대신 런타임 보정으로 대응했습니다
- **명시적 리소스 해제** — GL·오디오 리소스를 쥔 싱글턴을 static 소멸에 맡기면 macOS에서 종료 시 크래시가 납니다(GL 컨텍스트가 먼저 해체됨). `Game` 소멸자에서 창이 살아있는 동안 명시적으로 해제하도록 설계했습니다

구조도:

<img width="548" height="805" alt="structure" src="https://github.com/user-attachments/assets/5a6e26d3-bdf0-4112-be5a-0a72ae737a3d" />

## Patch note

#### (2026-08-28) macOS port & bugfixes
&nbsp; -> SFML 2 → 3.0.2 마이그레이션 (Homebrew 빌드 지원, mp3 로딩 문제 함께 해소)
&nbsp; -> 바닥 추락 사망 판정 복구, 프레임레이트 독립 물리, 레벨업 타이머 벽시계 기준으로 수정
&nbsp; -> 마이크 입력을 청크 단위 RMS + 시작 시 노이즈 플로어 자동 보정으로 재설계
&nbsp; -> 종료 시 SIGABRT 수정 (static 소멸 순서 — GL 리소스 명시 해제)

#### (2025-07-18 ~ 2025-09-28) Migration from Processing to C++
&nbsp; -> Convert the programming language to C++
&nbsp; -> With refactoring as OOP style

## Roadmap

&nbsp; -> Multi-playing
&nbsp; -> Score board
&nbsp; -> Destroying skill
&nbsp; -> Character skin selector
