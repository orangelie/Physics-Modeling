Web VPython 3.2


# Cannon Properties
Vm = 70.0                           # 포탄의 속력
Alpha = 25.0                        # y축과 포탄사이의 각
Gamma = 8.0                         # xz평면에서 x축과 포탄사이의 각
L = 65.0                            # 포탄의 길이
Yb = 10.0                           # 포탄의 밑면높이
sx = 0.0                            # 포탄의 x좌표
sy = Yb                             # 포탄의 y좌표
sz = 0.0                            # 포탄의 z좌표

# Target Properties
Length = 60.0                       # 목표물X 크기
Width = 50.0                        # 목표물Z 크기
Height = 80.0                       # 목표물Y 크기
X = 400.0                           # 목표물X 좌표
Y = 75.0                            # 목표물Y 좌표
Z = 45.0                            # 목표물Z 좌표

# Environment Properties
tInc = 0.1                          # 단위시간 증가분
time = 0.0                          # 발사순간의 시간
g = 9.80665                         # 중력가속도
pi = 3.14159265359                  # 파이
Interval = 60                       # 새로고침 빈도수


myScene = canvas(
        title = 'Examples of Cannon',
        autoscale = False,
        range = 800,
        width = 1280, height = 720,
        background = color.black)
        
bullet = sphere(canvas = myScene, radius = 10, pos = vec(sx, sy, sz), color = color.red)
target = box(canvas = myScene, size = vec(Length, Width, Height), pos = vec(X, Y, Z), color = color.green)

myScene.waitfor('click')

def DoSimulation(b, Lx, Ly, Lz, cosX, cosY, cosZ):
    xe = 0.0
    ze = 0.0
    tx1 = 0.0
    ty1 = 0.0
    tz1 = 0.0
    tx2 = 0.0
    ty2 = 0.0
    tz2 = 0.0
    
    xe = L * (cos((90 - Alpha) * (pi / 180)) * cos((Gamma) * (pi / 180)))
    ze = L * (cos((90 - Alpha) * (pi / 180)) * sin((Gamma) * (pi / 180)))
    
    sx = Vm * cosX * time + xe
    sy = (Yb + L * (cos(Alpha * (pi / 180)))) + (Vm * cosY * time) - (0.5 * g * time * time)
    sz = Vm * cosZ * time + ze
    
    tx1 = X - (Length / 2)
    tx2 = X + (Length / 2)
    ty1 = Y - (Height / 2)
    ty2 = Y + (Height / 2)
    tz1 = Z - (Width / 2)
    tz2 = Z + (Width / 2)
    
    # position update
    bullet.pos.x = sx
    bullet.pos.y = sy
    bullet.pos.z = sz
    
    if((sx >= tx1 and sx <= tx2) and (sy >= ty1 and sy <= ty2) and (sz >= tz1 and sz <= tz2)):
        return 1
        
    if(sy <= 0):
        return 2
    
    if(time > 3600):
        return 3
        
    return 0


b = L * (cos((90 - Alpha) * (pi / 180)))
Lx = b * (cos(Gamma * (pi / 180)))
Ly = L * (cos(Alpha * (pi / 180)))
Lz = b * (sin(Gamma * (pi / 180)))
    
cosX = Lx / L
cosY = Ly / L
cosZ = Lz / L
    
while True:
    rate(Interval)
    result = DoSimulation(b, Lx, Ly, Lz, cosX, cosY, cosZ)
    
    if(result == 0):
        pass
    else if(result == 1):
        print("타겟에맞음")
        break
    else if(result == 2):
        print("땅에떨어짐")
        break
    else if(result == 3):
        print("시간종료")
        break
    
    time = time + tInc