import pygame
import random

delta = 0
level = []

def lerp(start, end, t):
    return start + (end - start) * t
    
class Text:
    def __init__(self, size,font=None):
        self.font = pygame.font.Font(None, size)
        self.color = (255,255,255)

    def draw(self,text, x, y):
        text = self.font.render(text, True, self.color)
        screen.blit(text, (x, y))

    def draw2d(self,text,x,y):
        text = self.font.render(text, True, self.color)
        screen.blit(text, (x-camera.x, y-camera.y))

class Camera:
    def __init__(self, x, y):
        self.x = x
        self.y = y
        
    def move(self, x,y):
        self.x = lerp(self.x,x,0.1)
        self.y = lerp(self.y,y,0.1)
        
class Block:
    def __init__(self, x, y, texture, passable,bg):
        self.x = float(x)
        self.y = float(y)
        self.texture = int(texture)
        self.passable = int(passable)
        self.bg = int(bg)
        self.rect = pygame.Rect(self.x, self.y, 20, 20)
    
        
class Texture:
    def __init__(self, texture):
        self.img = pygame.image.load(texture).convert()
    #def draw(self,x,y,w,h):
    #    screen.blit(self.img, (x, y), (0, 0, w, h))
        
camera = Camera(0, 0)

def loadlevel(filename):
    l = []
    with open(filename) as f:
        l = f.read().split('"')
        for i in range(len(l)):
            j = l[i].split(';')
            l[i] = Block(j[1],j[2],j[0],j[4],j[3])
    return l

class Vector2:
    def __init__(self, x, y):
        self.x = x
        self.y = y

class Creature():
    def __init__(self, x, y,color,canDJ = False):
        self.x = x
        self.y = y
        self.color = color
        self.veloc = Vector2(0, 0)
        self.canDoubleJump = canDJ

    def move(self):
        self.veloc.y += 0.1
        self.veloc.x *= 0.97
        self.x += self.veloc.x * delta
        self.y += self.veloc.y * delta
        
    def collide():
        pass
        
    def draw(self):
        pygame.draw.rect(screen, self.color, pygame.Rect(310 + self.x - camera.x,230 + self.y - camera.y,20,40))

def color_surface(surface, color):
    arr = pygame.surfarray.array3d(surface)
    arr[:,:,0] = arr[:,:,0] * color[0]
    arr[:,:,1] = arr[:,:,1] * color[1]
    arr[:,:,2] = arr[:,:,2] * color[2]
    return pygame.surfarray.make_surface(arr)

def drawBlock(block):
    if block.x*20-camera.x < -20 or block.y*20-camera.y < -20 or block.x*20-camera.x > 640 or block.y*20-camera.y > 480:
        return
    im = textures[block.texture].img
    if block.bg:
        im = color_surface(im, (0.7,0.7,0.7))
        surf = bgsurf
    else:
        surf = fgsurf
    if block.texture == 3:
        im.set_alpha(128)
    if block.passable:
        h = 10
    else:
        h = 20
    surf.blit(im, (block.x*20-camera.x, block.y*20-camera.y), (0, 0, 20, h))
    if h == 10:
        im = color_surface(im, (0.7,0.7,0.7))
        bgsurf.blit(im, (block.x*20-camera.x, block.y*20-camera.y+10), (0, 10, 20, 20))

class Zombie(Creature):
    def __init__(self, x, y):
        super().__init__(x, y,"red",False)

pygame.init()
pygame.display.set_caption("SAM")
screen = pygame.display.set_mode((640, 480))
clock = pygame.time.Clock()
running = True
font = Text(20)

player = Creature(0,0,"lightgray")
enemies = []
textures = [Texture("assets/stone.png"), Texture("assets/dirt.png"), Texture("assets/grass.png"), Texture("assets/water.png"), Texture("assets/obsidian.png")]
level = loadlevel("level")

while running:
    
    keys = pygame.key.get_pressed()
    if keys[pygame.K_a]:
        player.veloc.x -= 0.2;
    if keys[pygame.K_d]:
        player.veloc.x += 0.2;
    
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False
        if event.type == pygame.KEYDOWN:
            if event.key == pygame.K_SPACE:
                player.veloc.y = -5;
            if event.key == pygame.K_z:
                enemies.append(Zombie(0,0))
    screen.fill("blue")
    
    bgsurf = pygame.Surface((640,480),pygame.SRCALPHA,32)
    bgsurf.convert_alpha()
    fgsurf = pygame.Surface((640,480),pygame.SRCALPHA,32)
    fgsurf.convert_alpha()
    for i in level:
        drawBlock(i)
    screen.blit(bgsurf, (0,0))
    player.move()
    camera.move(player.x, player.y)
    player.draw()
    font.draw2d("monkey",0,0)
    font.draw(f"FPS: {round(clock.get_fps())}", 0, 0)
    
    for i in enemies:
        i.move()
        i.draw()
        
    screen.blit(fgsurf, (0,0))
    
    pygame.display.flip()    
    delta = clock.tick(60) / 1000 * 60
    
pygame.quit()