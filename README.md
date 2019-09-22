# WattaSuit
2019 서울 하드웨어 해커톤 탑메이커 - 왔다슈트팀 Main Application Repository

## 팀명 및 팀원
### 팀명
왔다 슈트

### 팀원
* [HW] 김명규 - 설계 및 개발
* [SW] 김현재 - 설계 및 개발
* [Design] 전현선 - 기획 보강
* [Design] 송지민 - 제품 디자인

## 프로젝트 제목
실시간 자세 교정 제안 수영복 왔다슈트

## 프로젝트 목적
부정확한 자세를 센서를 통해 판단하고 수경에 부착된 display를 통해 자세를 교정하는 조언을 출력합니다.

## 파일 리스트
* inc/resource_oled.h
* src/analog-sensor.c
* src/resource_oled.c

## 코드 기여자
* src/resource_oled.c resource_OLED_disp_advise, resource_OLED_disp_StringFloat 현재
* src/analog-sensor.c 명규
* src/analog-sensor.c watch_timer_cb 현재

## 보드
* RPI3 : 센서 연동 및 디스플레이 출력, [github.com/WattaSuit/WattaSuit](github.com/WattaSuit/WattaSuit)

## 구현사항
* I2C / SPI 사용
* 클라우드 사용하지 않음
* 카메라 사용하지 않음
* 이미지 분석 사용하지 않음
