import pygame, sys
import random
import serial
import serial.tools.list_ports
import time

screen_w = 800
screen_h = 600

key_b  = 3
#串口端口列表
ports = serial.tools.list_ports.comports()
print(ports)
#获取可用端口并连接
if ports != []:
    for port in ports:
        print(port.device)
        ser = serial.Serial(port.device, baudrate=115200, timeout=10)
    key_b = 0
else:
    key_b = 1

print(key_b)

# pygame库的初始化
pygame.init()
# 设置窗口
screen_surface = pygame.display.set_mode((screen_w, screen_h))
pygame.display.set_caption("馨易康复--LMTX Game")
#地图
game_plaing_back_image = pygame.image.load('森林地图.png')#游戏进行中背景
base_pix_y = -screen_h
map_speed = 1

#显示要素
game_start_image =  pygame.image.load('开始钮.png')#开始按钮
game_arrow_image = pygame.image.load('箭头.png')#箭头
score_img = pygame.image.load('得分钮.png') 
game_continue_image = pygame.image.load('继续.png')
game_start_back_image = pygame.image.load('背景.png')#开始的静态背景
game_over_back_image  = pygame.image.load('背景.png')#结束的静态背景

#控制变量
GAME = True
game_start = False
music_begin_st = True
music_plaing_st = True
music_over_st= True

global LEFT_NUM
global RIGHT_NUM

#生命值
life = 3

#血条
lifes_image = pygame.image.load('血量.png')
def life_disp():
    life_left_pix = 20
    if life == 3:
        screen_surface.blit(lifes_image,(life_left_pix,10))
        screen_surface.blit(lifes_image,(life_left_pix+60,10))
        screen_surface.blit(lifes_image,(life_left_pix+120,10))
    elif life == 2:
        screen_surface.blit(lifes_image,(life_left_pix,10))
        screen_surface.blit(lifes_image,(life_left_pix+60,10))
    elif life == 1:
        screen_surface.blit(lifes_image,(life_left_pix,10))
    elif life == 0:
        game_start = False


# 判断是否为数字
def is_number(s):    
    try:    # 如果能运⾏ float(s) 语句，返回 True（字符串 s 是浮点数）        
        float(s)        
        return True    
    except ValueError:  # ValueError 为 Python 的⼀种标准异常，表⽰"传⼊⽆效的参数"        
        pass  # 如果引发了 ValueError 这种异常，不做任何事情（pass：不做任何事情，⼀般⽤做占位语句）    
    try:        
        import unicodedata  # 处理 ASCII 码的包        
        unicodedata.numeric(s)  # 把⼀个表⽰数字的字符串转换为浮点数返回的函数        
        return True    
    except (TypeError, ValueError):        
        pass    
        return False


# 创建一个飞机
class Plane(pygame.sprite.Sprite):
    def __init__(self,center):
        pygame.sprite.Sprite.__init__(self)  # 调用父类的初始化方法
        # super().__init__(self)#此方式调用不管事，原因未知
        # self.image = pygame.Surface((50,50))
        # self.image.fill('green')
        self.image = pygame.image.load('黄色飞机.png')
        self.rect = self.image.get_rect()
        # 设置初始位置
        self.rect.center = (center)
    
    
    def update(self):
        
        # 获取键盘事件，如果按下，返回True
        if key_b == 1:   
            keys = pygame.key.get_pressed()  # 类似字典，通过key获取value
            if keys[pygame.K_LEFT] and self.rect.left > 0:
                self.rect = self.rect.move(-10, 0)
            if keys[pygame.K_RIGHT] and self.rect.right < screen_surface.get_width():
                self.rect = self.rect.move(10, 0)
            if keys[pygame.K_UP] and self.rect.top > 0:
                self.rect = self.rect.move(0, -10)
            if keys[pygame.K_DOWN] and self.rect.bottom < screen_surface.get_height():
                self.rect = self.rect.move(0, 10)
        
        screen_surface.blit(self.image, self.rect)  # 屏幕上 放 飞机，位置是rect的位置
        


# 创建一个子弹类Bullet
class Bullet(pygame.sprite.Sprite):
    def __init__(self, center):  # center是初始位置，传入飞机的中心
        pygame.sprite.Sprite.__init__(self)  # 调用父类的初始化方法
        # self.image = pygame.Surface((10,30))
        # self.image.fill('yellow')
        self.image = pygame.image.load('bt.png')
        self.rect = self.image.get_rect()
        # 设置初始位置 初始位置要在飞机处——>外部传进
        self.rect.center = center

    def update(self):
        
        self.rect = self.rect.move(0, -10)
        screen_surface.blit(self.image, self.rect)
        if self.rect.top < 0:  # 判断是否出屏幕
            self.kill()  # 终止，相当于删除子弹


# 创建敌机1
class Enemy_plane1(pygame.sprite.Sprite):
    def __init__(self):
        pygame.sprite.Sprite.__init__(self)  # 调用父类的初始化方法
        # super().__init__(self)#此方式调用不管事，原因未知
        # self.image = pygame.Surface((50,50))
        # self.image.fill('red')
        self.image = pygame.image.load('红色敌机.png')
        self.rect = self.image.get_rect()
        # 设置初始位置
        x = random.randint(10, screen_surface.get_width()-10)
        y = 0
        self.rect.center = (x, y)

    def update(self):
        self.rect = self.rect.move((0,2))
        if self.rect.y > screen_surface.get_height():
            x = random.randint(10, screen_surface.get_width()-10)
            y = 0
            self.rect.center = (x, y)
        screen_surface.blit(self.image, self.rect)  # 屏幕上 放 飞机，位置是rect的位置

# 创建敌机2
class Enemy_plane2(pygame.sprite.Sprite):
    def __init__(self):
        pygame.sprite.Sprite.__init__(self)  # 调用父类的初始化方法
        # super().__init__(self)#此方式调用不管事，原因未知
        # self.image = pygame.Surface((50,50))
        # self.image.fill('red')
        self.image = pygame.image.load('黄色敌机.png')
        self.rect = self.image.get_rect()
        # 设置初始位置
        x = random.randint(10, screen_surface.get_width()-20)
        y = -10
        self.rect.center = (x, y)

    def update(self):
        self.rect = self.rect.move((0, 2))
        if self.rect.y > screen_surface.get_height():
            x = random.randint(10, screen_surface.get_width()-20)
            y = -10
            self.rect.center = (x, y)
        screen_surface.blit(self.image, self.rect)  # 屏幕上 放 飞机，位置是rect的位置

# 创建敌机3
class Enemy_plane3(pygame.sprite.Sprite):
    def __init__(self):
        pygame.sprite.Sprite.__init__(self)  # 调用父类的初始化方法
        # super().__init__(self)#此方式调用不管事，原因未知
        # self.image = pygame.Surface((50,50))
        # self.image.fill('red')
        self.image = pygame.image.load('红色飞机.png')
        self.rect = self.image.get_rect()
        # 设置初始位置
        x = random.randint(10, screen_surface.get_width()-50)
        y = -10
        self.rect.center = (x, y)

    def update(self):
        self.rect = self.rect.move((0, 2))
        if self.rect.y > screen_surface.get_height():
            x = random.randint(10, screen_surface.get_width()-50)
            y = -50
            self.rect.center = (x, y)
        screen_surface.blit(self.image, self.rect)  # 屏幕上 放 飞机，位置是rect的位置

# 创建一架飞机
plane_surface = Plane(center=(screen_surface.get_width()/2,screen_surface.get_height()-100))
# 创建一个 子弹 精灵组（用于存放所有的精灵对象）
bullet_sprite = pygame.sprite.Group()
# 创建敌机
enemy_plane_surface1 = Enemy_plane1()
enemy_plane_surface2 = Enemy_plane2()
enemy_plane_surface3 = Enemy_plane3()
score = 0  # 存储得分

#背景音乐
music_begin = pygame.mixer.Sound('开场音乐.mp3')#开场音乐
music_bg = pygame.mixer.Sound('游戏中.mp3')  # 加载音乐
music_over = pygame.mixer.Sound('结束音效.mp3')
music_overs = pygame.mixer.Sound('结束音乐.mp3')
music_wink = pygame.mixer.Sound('弹出声.mp3')
music_crash = pygame.mixer.Sound('碰撞.mp3')

#发射音效
music_shoot = pygame.mixer.Sound('shoot.mp3')  # 加载音效
#得分音效
music_score = pygame.mixer.Sound('爆炸.mp3')  # 加载音效
#字体
font_name1 = pygame.font.match_font("宋体")  # 2.获得字体文件
font1 = pygame.font.Font(font_name1, 80)  # 1.获取font对象（需要字体文件）
font_name2 = pygame.font.match_font("黑体")  # 2.获得字体文件
font2 = pygame.font.Font(font_name2, 40)  # 1.获取font对象（需要字体文件）

clock = pygame.time.Clock()
# 3.游戏的主循环（事件的监听，数据的修改及绘图，界面的更新）
while True:
    while GAME:
        
        #游戏开始前
        if game_start == False:
            screen_surface.blit(game_start_back_image,(0,base_pix_y))#背景滚动
            screen_surface.blit(game_start_image,(300,200))#开始按钮
            screen_surface.blit(game_arrow_image,(200,200))#箭头位置
            font_surface1 = font2.render(str('Please press Enter to continue...'),False,'white')
            screen_surface.blit(font_surface1, (200,300))
            #单曲循环
            if music_begin_st == True:
                music_begin.set_volume(0.3)
                music_begin.play(-1)
                music_begin_st = False

            base_pix_y += 0.5
            if base_pix_y>=0:
                base_pix_y = -screen_h
            pygame.display.update()#刷新界面
            for event in pygame.event.get():#遍历pygame事件
                if event.type == pygame.QUIT :#如果是退出窗口
                    pygame.quit()
                    sys.exit()
                elif event.type == pygame.KEYDOWN and event.key == pygame.K_RETURN:
                    game_start = True 
                    if key_b == 0:
                        ser.write(b'START\r\n')
                    plane_surface.__init__(center=(screen_surface.get_width()/2,screen_surface.get_height()-100))
                    music_begin.stop()
                    music_wink.play()
                    time.sleep(0.5)
        #游戏开始后
        elif game_start == True:
            
            if music_plaing_st == True:
                music_bg.set_volume(0.5)
                music_bg.play(-1)
                music_plaing_st = False

            # 3.1 事件的监听，关闭按钮无法使用。
            for event in pygame.event.get():
                if event.type == pygame.QUIT:
                    if key_b == 0:
                        ser.write(b'END\r\n')
                    time.sleep(0.5)
                    sys.exit()
                elif event.type == pygame.KEYDOWN and event.key == pygame.K_SPACE:
                    if key_b == 1:
                        # 按下一个空格，创建一个子弹对象
                        bullet_surface = Bullet(plane_surface.rect.center)
                        bullet_sprite.add(bullet_surface)
                        music_shoot.set_volume(0.3)  # 播放发射音效
                        music_shoot.play()  # 播放发射音效


            # 3.2 数据的修改及绘图
            #screen_surface.fill((0, 0, 0))  # 重新绘制背景
            screen_surface.blit(game_plaing_back_image,(0,base_pix_y))
            base_pix_y += map_speed
            if base_pix_y>=0:
                base_pix_y = -screen_h
            if key_b == 0:
                message = ser.readline()
                if (message != b'') and (len(message) <= 10) and is_number(message):
                    num = 4*int(message)
                    #print(num)
                    if num >8:
                        plane_surface.rect.x -= num
                        
                        bullet_surface = Bullet(plane_surface.rect.center)
                        bullet_sprite.add(bullet_surface)
                        music_shoot.set_volume(0.3)  # 播放发射音效
                        music_shoot.play()  # 播放发射音效
                        if plane_surface.rect.left < 0:
                            plane_surface.rect.left = 0 
                    elif num <-8 :
                        plane_surface.rect.x -= num
                        bullet_surface = Bullet(plane_surface.rect.center)
                        bullet_sprite.add(bullet_surface)
                        music_shoot.set_volume(0.3)  # 播放发射音效
                        music_shoot.play()  # 播放发射音效
                        if plane_surface.rect.right > screen_surface.get_width():
                            plane_surface.rect.right = screen_surface.get_width()
            # 3.2 飞机修改数据 更新画面
            plane_surface.update()  # 修改数据+更新画面
            # 3.2 子弹更新数据 更新画面
            bullet_sprite.update()  # 修改数据+更新画面
            # 3.2 敌机更新数据 更新画面
            enemy_plane_surface1.update()
            enemy_plane_surface2.update()
            enemy_plane_surface3.update()

            #  判断子弹是否和敌机碰撞
            a1 = pygame.sprite.spritecollide(enemy_plane_surface1, bullet_sprite, True, collided=pygame.sprite.collide_mask)
            a2 = pygame.sprite.spritecollide(enemy_plane_surface2, bullet_sprite, True, collided=pygame.sprite.collide_mask)
            a3 = pygame.sprite.spritecollide(enemy_plane_surface3, bullet_sprite, True, collided=pygame.sprite.collide_mask)
            
            if a1:
                music_score.play()  # 播放得分音效
                score += 10
                x = random.randint(0, screen_surface.get_width())
                y = 0
                enemy_plane_surface1.rect.center = (x, y)
            if a2:
                music_score.play()  # 播放得分音效
                score += 5
                x = random.randint(0, screen_surface.get_width()-20)
                y = -10
                enemy_plane_surface2.rect.center = (x, y)
            
            if a3:
                music_score.play()  # 播放得分音效
                score += 20
                x = random.randint(0, screen_surface.get_width()-50)
                y = -50
                enemy_plane_surface3.rect.center = (x, y)

            #  判断敌机和战斗机是否碰撞
            b1 = pygame.sprite.collide_mask(enemy_plane_surface1, plane_surface)  # 返回布尔值
            b2 = pygame.sprite.collide_mask(enemy_plane_surface2, plane_surface)  # 返回布尔值
            b3 = pygame.sprite.collide_mask(enemy_plane_surface3, plane_surface)  # 返回布尔值
            if b1 or b2 or b3:
                life -= 1
                music_crash.play()
                x = random.randint(0, screen_surface.get_width())
                y = 0
                if b1:
                    enemy_plane_surface1.rect.center = (x, y)
                if b2:
                    enemy_plane_surface2.rect.center = (x, y)
                if b3:
                    enemy_plane_surface3.rect.center = (x, y)

                if life == 0:
                    GAME = False
                    if key_b == 0:
                        ser.write(b'END\r\n')
                    music_bg.stop()
                    if music_over_st == True:
                        music_overs.set_volume(0.3)
                        music_overs.play(-1)
                        music_over_st = False
                        screen_surface.blit(game_continue_image,(250,300))
                        screen_surface.blit(score_img,(300,200))
                        screen_surface.blit(font_surface, (400,195))
                        font_surface1 = font2.render(str('Please press Enter to continue...'),False,'white')
                        screen_surface.blit(font_surface1, (200,400))
                        
                

            # 血条更新
            life_disp()
            # 实现记录分数
            # 绘制内容：text为内容，True为是否抗锯齿， WHITE是字体颜色
            font_surface = font1.render(str(score),False,'red')  # 3.将文字生成 surface对象
            screen_surface.blit(score_img,(630,20))
            screen_surface.blit(font_surface, (700,15))

        # 3.3 界面的更新
        pygame.display.flip()
        pygame.display.update()#刷新界面
        clock.tick(60)  # 

    for event in pygame.event.get():#遍历pygame事件
        if event.type == pygame.QUIT:#如果是退出窗口
            if key_b == 0:
                ser.write(b'END\r\n')
            pygame.quit()
            sys.exit()


        elif event.type == pygame.KEYDOWN and event.key == pygame.K_RETURN:
            GAME = True
            game_start = False
            music_begin_st = True
            music_plaing_st = True
            music_over_st= True
            score = 0
            life = 3
            music_overs.stop()
            
