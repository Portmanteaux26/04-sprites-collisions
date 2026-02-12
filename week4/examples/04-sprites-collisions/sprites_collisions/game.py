from __future__ import annotations

from dataclasses import dataclass, field

import random

import pygame


@dataclass(frozen=True)
class Palette:
    bg: pygame.Color = field(default_factory=lambda: pygame.Color("#1e222a"))
    panel: pygame.Color = field(default_factory=lambda: pygame.Color("#2a303c"))
    text: pygame.Color = field(default_factory=lambda: pygame.Color("#e5e9f0"))
    subtle: pygame.Color = field(default_factory=lambda: pygame.Color("#a3adbf"))

    player: pygame.Color = field(default_factory=lambda: pygame.Color("#88c0d0"))
    coin: pygame.Color = field(default_factory=lambda: pygame.Color("#ebcb8b"))
    hazard: pygame.Color = field(default_factory=lambda: pygame.Color("#bf616a"))
    wall: pygame.Color = field(default_factory=lambda: pygame.Color("#4c566a"))


def _clamp(value: float, lo: float, hi: float) -> float:
    return max(lo, min(hi, value))


class Wall(pygame.sprite.Sprite):
    def __init__(self, rect: pygame.Rect, color: pygame.Color) -> None:
        super().__init__()
        self.rect = rect.copy()
        self.color = color


class Coin(pygame.sprite.Sprite):
    def __init__(
        self,
        center: tuple[int, int],
        *,
        hitbox_size: int = 18,
        visual_size: int = 30,
        color: pygame.Color,
    ) -> None:
        super().__init__()
        self.rect = pygame.Rect(0, 0, hitbox_size, hitbox_size)
        self.rect.center = center

        self.visual_size = visual_size
        self.color = color


class Hazard(pygame.sprite.Sprite):
    def __init__(
        self,
        center: tuple[int, int],
        *,
        size: int = 28,
        color: pygame.Color,
        patrol_dx: int = 140,
        speed: float = 180.0,
    ) -> None:
        super().__init__()
        self.rect = pygame.Rect(0, 0, size, size)
        self.rect.center = center
        self.color = color

        self.home = pygame.Vector2(center)
        self.patrol_dx = patrol_dx
        self.speed = speed

        self.direction = 1

    def update(self, dt: float) -> None:
        x = self.rect.centerx + self.direction * self.speed * dt
        if x < self.home.x - self.patrol_dx:
            x = self.home.x - self.patrol_dx
            self.direction = 1
        elif x > self.home.x + self.patrol_dx:
            x = self.home.x + self.patrol_dx
            self.direction = -1

        self.rect.centerx = int(x)


class ScorePopup(pygame.sprite.Sprite):
    """Floating '+1' text that rises and fades after a coin pickup."""

    def __init__(self, center: tuple[int, int], font: pygame.font.Font) -> None:
        super().__init__()
        self._font = font
        self._x = float(center[0])
        self._y = float(center[1])
        self._alpha = 255.0
        self._life = 0.8
        self._elapsed = 0.0
        self._render()

    def _render(self) -> None:
        base = self._font.render("+1", True, pygame.Color("#a3be8c"))
        self.image = base.convert_alpha()
        self.image.set_alpha(int(self._alpha))
        self.rect = self.image.get_rect(center=(round(self._x), round(self._y)))

    def update(self, dt: float) -> None:
        self._elapsed += dt
        self._y -= 45 * dt
        self._alpha = max(0.0, 255.0 * (1.0 - self._elapsed / self._life))
        if self._elapsed >= self._life:
            self.kill()
            return
        self._render()


class Player(pygame.sprite.Sprite):
    def __init__(
        self,
        center: tuple[int, int],
        *,
        hitbox_size: int = 28,
        visual_size: int = 38,
        color: pygame.Color,
    ) -> None:
        super().__init__()
        self.rect = pygame.Rect(0, 0, hitbox_size, hitbox_size)
        self.rect.center = center

        self.visual_size = visual_size
        self.color = color

        self.vel = pygame.Vector2(0, 0)
        self.speed = 320.0

        self.hp = 3
        self.invincible_for = 0.0

        self.score = 0

    @property
    def is_invincible(self) -> bool:
        return self.invincible_for > 0


class Game:
    fps = 60

    SCREEN_W, SCREEN_H = 960, 540
    HUD_H = 56
    PADDING = 12

    def __init__(self) -> None:
        self.palette = Palette()

        self.screen = pygame.display.set_mode((self.SCREEN_W, self.SCREEN_H))
        self.font = pygame.font.SysFont(None, 22)
        self.big_font = pygame.font.SysFont(None, 40)

        self.screen_rect = pygame.Rect(0, 0, self.SCREEN_W, self.SCREEN_H)
        self.playfield = pygame.Rect(
            self.PADDING,
            self.HUD_H + self.PADDING,
            self.SCREEN_W - 2 * self.PADDING,
            self.SCREEN_H - self.HUD_H - 2 * self.PADDING,
        )

        self.debug = False
        self.state = "title"  # title | play | gameover

        self.all_sprites: pygame.sprite.Group[pygame.sprite.Sprite] = pygame.sprite.Group()
        self.walls: pygame.sprite.Group[Wall] = pygame.sprite.Group()
        self.coins: pygame.sprite.Group[Coin] = pygame.sprite.Group()
        self.hazards: pygame.sprite.Group[Hazard] = pygame.sprite.Group()
        self.popups: pygame.sprite.Group[ScorePopup] = pygame.sprite.Group()

        self.player = Player(self.playfield.center, color=self.palette.player)
        self.all_sprites.add(self.player)

        self._shake = 0.0
        self._reset_level(keep_state=True)

    def _reset_level(self, *, keep_state: bool = False) -> None:
        self.all_sprites.empty()
        self.walls.empty()
        self.coins.empty()
        self.hazards.empty()
        self.popups.empty()

        self.player = Player(self.playfield.center, color=self.palette.player)
        self.all_sprites.add(self.player)

        def add_wall(r: pygame.Rect) -> None:
            wall = Wall(r, self.palette.wall)
            self.walls.add(wall)
            self.all_sprites.add(wall)

        t = 16
        # Arena boundary (solid)
        add_wall(pygame.Rect(self.playfield.left, self.playfield.top, self.playfield.width, t))
        add_wall(pygame.Rect(self.playfield.left, self.playfield.bottom - t, self.playfield.width, t))
        add_wall(pygame.Rect(self.playfield.left, self.playfield.top, t, self.playfield.height))
        add_wall(pygame.Rect(self.playfield.right - t, self.playfield.top, t, self.playfield.height))

        # Interior walls (solid)
        add_wall(pygame.Rect(self.playfield.left + 120, self.playfield.top + 90, 18, 280))
        add_wall(pygame.Rect(self.playfield.left + 380, self.playfield.top + 40, 18, 240))
        add_wall(pygame.Rect(self.playfield.left + 540, self.playfield.top + 220, 240, 18))

        # Hazards (damage)
        h1 = Hazard((self.playfield.centerx + 180, self.playfield.centery - 80), color=self.palette.hazard)
        h2 = Hazard(
            (self.playfield.centerx - 140, self.playfield.centery + 140),
            color=self.palette.hazard,
            patrol_dx=110,
            speed=220.0,
        )
        self.hazards.add(h1, h2)
        self.all_sprites.add(h1, h2)

        # Coins (trigger)
        rng = random.Random(4)
        for _ in range(8):
            for __ in range(100):
                x = rng.randint(self.playfield.left + 40, self.playfield.right - 40)
                y = rng.randint(self.playfield.top + 40, self.playfield.bottom - 40)
                candidate = Coin((x, y), color=self.palette.coin)

                if pygame.sprite.spritecollideany(candidate, self.walls):
                    continue
                if pygame.sprite.spritecollideany(candidate, self.coins):
                    continue
                if candidate.rect.colliderect(self.player.rect):
                    continue

                self.coins.add(candidate)
                self.all_sprites.add(candidate)
                break

        if not keep_state:
            self.state = "play"

    def handle_event(self, event: pygame.event.Event) -> None:
        if event.type != pygame.KEYDOWN:
            return

        if event.key == pygame.K_ESCAPE:
            pygame.event.post(pygame.event.Event(pygame.QUIT))
            return

        if event.key == pygame.K_F1:
            self.debug = not self.debug
            return

        if event.key == pygame.K_r:
            self._reset_level(keep_state=(self.state == "title"))
            return

        if self.state in {"title", "gameover"} and event.key == pygame.K_SPACE:
            self._reset_level(keep_state=True)
            self.state = "play"

    def _read_move(self) -> pygame.Vector2:
        keys = pygame.key.get_pressed()

        x = 0
        y = 0

        if keys[pygame.K_LEFT] or keys[pygame.K_a]:
            x -= 1
        if keys[pygame.K_RIGHT] or keys[pygame.K_d]:
            x += 1
        if keys[pygame.K_UP] or keys[pygame.K_w]:
            y -= 1
        if keys[pygame.K_DOWN] or keys[pygame.K_s]:
            y += 1

        v = pygame.Vector2(x, y)
        if v.length_squared() > 0:
            v = v.normalize()
        return v

    def _move_player_axis(self, axis: str, amount: float) -> None:
        if axis == "x":
            self.player.rect.x += int(round(amount))
        else:
            self.player.rect.y += int(round(amount))

        hits = pygame.sprite.spritecollide(self.player, self.walls, dokill=False)
        if not hits:
            return

        for wall in hits:
            if axis == "x":
                if amount > 0:
                    self.player.rect.right = wall.rect.left
                elif amount < 0:
                    self.player.rect.left = wall.rect.right
            else:
                if amount > 0:
                    self.player.rect.bottom = wall.rect.top
                elif amount < 0:
                    self.player.rect.top = wall.rect.bottom

    def _apply_damage(self, source_rect: pygame.Rect) -> None:
        if self.player.is_invincible:
            return

        self.player.hp -= 1
        self.player.invincible_for = 0.85

        push = pygame.Vector2(self.player.rect.center) - pygame.Vector2(source_rect.center)
        if push.length_squared() == 0:
            push = pygame.Vector2(1, 0)
        push = push.normalize() * 520.0
        self.player.vel.update(push)

        self._shake = 0.18

        if self.player.hp <= 0:
            self.state = "gameover"

    def update(self, dt: float) -> None:
        if self._shake > 0:
            self._shake = max(0.0, self._shake - dt)

        if self.state != "play":
            return

        move = self._read_move()
        self.player.vel.update(move * self.player.speed)

        # Axis-separated movement against solid walls
        self._move_player_axis("x", self.player.vel.x * dt)
        self._move_player_axis("y", self.player.vel.y * dt)

        # Triggers: coin pickup
        picked = pygame.sprite.spritecollide(self.player, self.coins, dokill=True)
        if picked:
            self.player.score += len(picked)
            for coin in picked:
                self.popups.add(ScorePopup(coin.rect.center, self.font))

        # Hazards: damage + response
        for hz in pygame.sprite.spritecollide(self.player, self.hazards, dokill=False):
            self._apply_damage(hz.rect)

        self.hazards.update(dt)
        self.popups.update(dt)

        if self.player.invincible_for > 0:
            self.player.invincible_for = max(0.0, self.player.invincible_for - dt)

        if len(self.coins) == 0:
            # Quick win condition: respawn coins + hazards to keep playing
            self._reset_level(keep_state=True)
            self.state = "play"

    def _camera_offset(self) -> pygame.Vector2:
        if self._shake <= 0:
            return pygame.Vector2(0, 0)

        strength = 9.0 * (self._shake / 0.18)
        return pygame.Vector2(
            random.uniform(-strength, strength),
            random.uniform(-strength, strength),
        )

    def draw(self) -> None:
        self.screen.fill(self.palette.bg)

        pygame.draw.rect(self.screen, self.palette.panel, pygame.Rect(0, 0, self.SCREEN_W, self.HUD_H))
        pygame.draw.line(
            self.screen,
            pygame.Color("#000000"),
            (0, self.HUD_H),
            (self.SCREEN_W, self.HUD_H),
            1,
        )

        hud = f"Score: {self.player.score}    HP: {self.player.hp}"
        if self.player.is_invincible:
            hud += "    i-frames"

        self.screen.blit(self.font.render(hud, True, self.palette.text), (14, 18))
        self.screen.blit(
            self.font.render("WASD/Arrows move • F1 debug • R reset • Esc quit", True, self.palette.subtle),
            (14, 36),
        )

        cam = self._camera_offset()

        # Draw walls
        for wall in self.walls:
            pygame.draw.rect(self.screen, wall.color, wall.rect.move(cam))

        # Draw coins (bigger art than hitbox)
        for coin in self.coins:
            visual = pygame.Rect(0, 0, coin.visual_size, coin.visual_size)
            visual.center = coin.rect.center
            pygame.draw.circle(self.screen, coin.color, visual.center + cam, visual.width // 2)
            pygame.draw.circle(self.screen, pygame.Color("#000000"), visual.center + cam, visual.width // 2, 2)

        # Draw hazards
        for hazard in self.hazards:
            r = hazard.rect.move(cam)
            pts = [(r.centerx, r.top), (r.right, r.bottom), (r.left, r.bottom)]
            pygame.draw.polygon(self.screen, hazard.color, pts)
            pygame.draw.polygon(self.screen, pygame.Color("#000000"), pts, 2)

        # Draw player (bigger art than hitbox)
        pr = self.player.rect.move(cam)
        visual = pygame.Rect(0, 0, self.player.visual_size, self.player.visual_size)
        visual.center = pr.center
        player_color = self.player.color
        if self.player.is_invincible:
            # Simple blink while invincible
            if int(self.player.invincible_for * 16) % 2 == 0:
                player_color = pygame.Color("#d8dee9")
        pygame.draw.circle(self.screen, player_color, visual.center, visual.width // 2)
        pygame.draw.circle(self.screen, pygame.Color("#000000"), visual.center, visual.width // 2, 2)

        if self.debug:
            self._draw_debug(cam)

        # Popups drawn in screen space
        for popup in self.popups:
            self.screen.blit(popup.image, popup.rect)

        if self.state == "title":
            self._draw_center_message("Sprites + Collisions\nPress Space to start", cam)
        elif self.state == "gameover":
            self._draw_center_message("Game over\nPress Space to restart", cam)

    def _draw_debug(self, cam: pygame.Vector2) -> None:
        # Hitboxes
        pygame.draw.rect(self.screen, pygame.Color("#8fbcbb"), self.player.rect.move(cam), 2)
        for coin in self.coins:
            pygame.draw.rect(self.screen, pygame.Color("#ebcb8b"), coin.rect.move(cam), 2)
        for hazard in self.hazards:
            pygame.draw.rect(self.screen, pygame.Color("#bf616a"), hazard.rect.move(cam), 2)

        # Help text
        self.screen.blit(
            self.font.render("DEBUG: Rect hitboxes (collisions use these)", True, self.palette.text),
            (self.SCREEN_W - 320, 18),
        )

    def _draw_center_message(self, message: str, cam: pygame.Vector2) -> None:
        lines = message.split("\n")
        total_h = len(lines) * 44
        y = self.playfield.centery - total_h // 2

        overlay = pygame.Surface((self.playfield.width, self.playfield.height), pygame.SRCALPHA)
        overlay.fill((0, 0, 0, 150))
        self.screen.blit(overlay, self.playfield.topleft + cam)

        for line in lines:
            surf = self.big_font.render(line, True, self.palette.text)
            x = self.playfield.centerx - surf.get_width() // 2
            self.screen.blit(surf, (x, y) + cam)
            y += 44
