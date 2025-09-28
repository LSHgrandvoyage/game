이승현의 야심찬 첫 프로젝트

목소리로 비행기를 움직여 소행성을 피하자

## Structure
/game
├── /src # source code
│ ├── /managers # Resource management code
│ │ ├── AudioManager.cpp
│ │ ├── AudioManager.h
│ │ ├── ResourceManager.cpp
│ │ └── ResourceManager.h
│ │
│ ├── /game # Game core code
│ │ ├── Game.cpp
│ │ └── Game.h
│ │
│ ├── /objects # Game object classes
│ │ ├── GameObject.h
│ │ ├── Obstacle.cpp
│ │ ├── Obstacle.h
│ │ ├── Spaceship.cpp
│ │ └── Spaceship.h
│ │
│ └── main.cpp
│
└── /data # Game resource
├── /audio
├── /fonts
└── /textures

## Patch note
#### (2025-07-18 ~ 2025-09-28) Migration from Processing to C++
##### What I have to do  
&nbsp; -> Convert the programming language to C++  
&nbsp; -> With refactoring as OOP style


##### Further function
&nbsp; -> Multi-playing  
&nbsp; -> Score board  
&nbsp; -> Destroying skill  
&nbsp; -> Character skin selector
