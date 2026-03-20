import serial
import time
import keyboard  # 키보드 입력을 감지하는 라이브러리

# --- 설정 ---
PORT = 'COM3'      # 아두이노 포트
BAUD = 9600        # 보드레이트

# 한 번 통신할 때 보낼 기본 이동량 (환경에 맞게 조절하세요)
STEP_VAL = 20      # 스텝 모터: 한 번에 20스텝씩 이동
DC_VAL = 200       # DC 모터: 한 번에 200ms씩 구동

try:
    ser = serial.Serial(PORT, BAUD, timeout=0.05)
    print(f"[{PORT}] 아두이노와 연결되었습니다!")
except Exception as e:
    print(f"연결 실패: {e}")
    exit()

print("=====================================")
print("🎮 키보드 제어를 시작합니다. (종료: ESC)")
print(" [스텝 모터]")
print(" - X축: 상/하 방향키")
print(" - Y축: 좌/우 방향키")
print(" [DC 모터]")
print(" - Z1: '1' (Forward) / 'Q' (Backward)")
print(" - Z2: '2' (Forward) / 'W' (Backward)")
print(" - Z3: '3' (Forward) / 'E' (Backward)")
print(" - Z4: '4' (Forward) / 'R' (Backward)")
print("=====================================")

try:
    while True:
        # ESC 키를 누르면 프로그램 종료
        if keyboard.is_pressed('esc'):
            print("프로그램을 종료합니다.")
            break

        # 초기값 0으로 리셋
        dx, dy, dz1, dz2, dz3, dz4 = 0, 0, 0, 0, 0, 0
        is_pressed = False

        # 1. 스텝 모터 (X, Y) 제어
        if keyboard.is_pressed('up'):
            dx = STEP_VAL
            is_pressed = True
        elif keyboard.is_pressed('down'):
            dx = -STEP_VAL
            is_pressed = True

        if keyboard.is_pressed('left'):
            dy = STEP_VAL
            is_pressed = True
        elif keyboard.is_pressed('right'):
            dy = -STEP_VAL
            is_pressed = True

        # 2. DC 모터 (Z1 ~ Z4) 제어
        if keyboard.is_pressed('1'): dz1 = DC_VAL; is_pressed = True
        elif keyboard.is_pressed('q'): dz1 = -DC_VAL; is_pressed = True

        if keyboard.is_pressed('2'): dz2 = DC_VAL; is_pressed = True
        elif keyboard.is_pressed('w'): dz2 = -DC_VAL; is_pressed = True

        if keyboard.is_pressed('3'): dz3 = DC_VAL; is_pressed = True
        elif keyboard.is_pressed('e'): dz3 = -DC_VAL; is_pressed = True

        if keyboard.is_pressed('4'): dz4 = DC_VAL; is_pressed = True
        elif keyboard.is_pressed('r'): dz4 = -DC_VAL; is_pressed = True

        # 아무 키나 눌렸을 때만 시리얼 전송
        if is_pressed:
            command = f"{dx},{dy},{dz1},{dz2},{dz3},{dz4}\n"
            ser.write(command.encode('utf-8'))
            print(f"전송: {command.strip()}")
            
            # 아두이노가 처리할 수 있도록 약간의 딜레이 (데이터 병목 방지)
            time.sleep(0.05) 
            
except KeyboardInterrupt:
    print("\n강제 종료되었습니다.")
finally:
    # 종료 시 모터들이 멈추도록 0을 한 번 보내주고 포트를 닫습니다.
    if ser.is_open:
        ser.write("0,0,0,0,0,0\n".encode('utf-8'))
        ser.close()
        print("시리얼 포트를 닫았습니다.")