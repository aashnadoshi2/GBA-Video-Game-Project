#include "gba.h"
#include <stdio.h>
#include <stdlib.h>
#include "main.h"

volatile unsigned short *videoBuffer = (volatile unsigned short *) 0x6000000;
u32 vBlankCounter = 0;

void waitForVBlank(void) {

  // TODO: IMPLEMENT

  // (1)
  // Write a while loop that loops until we're NOT in vBlank anymore:
  // (This prevents counting one VBlank more than once if your app is too fast)

  // (2)
  // Write a while loop that keeps going until we're in vBlank:

  // (3)
  // Finally, increment the vBlank counter:
  while (SCANLINECOUNTER > 160);
  while (SCANLINECOUNTER < 160)
  vBlankCounter++;
}

static int __qran_seed = 42;
static int qran(void) {
  __qran_seed = 1664525 * __qran_seed + 1013904223;
  return (__qran_seed >> 16) & 0x7FFF;
}

int randint(int min, int max) { return (qran() * (max - min) >> 15) + min; }

void setPixel(int row, int col, u16 color) {
  // TODO: IMPLEMENT
  UNUSED(row);
  UNUSED(col);
  UNUSED(color);

   *(videoBuffer + OFFSET(row, col, WIDTH) ) = color;
}

void drawRectDMA(int row, int col, int width, int height, volatile u16 color) {
  // TODO: IMPLEMENT
  UNUSED(row);
  UNUSED(col);
  UNUSED(width);
  UNUSED(height);
  UNUSED(color);

  for (int r = 0; r < height; r++) {
    DMA[3].src = &color;
    DMA[3].dst = &videoBuffer[OFFSET(row + r, col, WIDTH)];
    DMA[3].cnt = width | DMA_ON | DMA_SOURCE_FIXED;
  }
}

void drawFullScreenImageDMA(const u16 *image) {
  // TODO: IMPLEMENT
  UNUSED(image);
  drawImageDMA(0, 0, WIDTH, HEIGHT, image);
}

void drawImageDMA(int row, int col, int width, int height, const u16 *image) {
  // TODO: IMPLEMENT
  UNUSED(row);
  UNUSED(col);
  UNUSED(width);
  UNUSED(height);
  UNUSED(image);

  for(int r = 0; r < height; r++) {
  	DMA[3].src = &image[OFFSET(r, 0, width)];
  	DMA[3].dst = &videoBuffer[OFFSET(row + r, col, WIDTH)];
  	DMA[3].cnt = width | DMA_ON | DMA_SOURCE_INCREMENT | DMA_DESTINATION_INCREMENT;
  }
}

void undrawImageDMA(int row, int col, int width, int height, const u16 *image) {
  // TODO: IMPLEMENT
  UNUSED(row);
  UNUSED(col);
  UNUSED(width);
  UNUSED(height);
  UNUSED(image);

  // don't need to complete!
  // for(int r = 0; r < height; r++) {
  // 	DMA[3].src = &image[OFFSET(r, 0, WIDTH)]; //is the width supposed to be size of screen (WIDTH) or param width?
  // 	DMA[3].dst = &videoBuffer[OFFSET(row + r, col, WIDTH)];
  // 	DMA[3].cnt = width | DMA_ON | DMA_SOURCE_INCREMENT | DMA_DESTINATION_INCREMENT;
}

void fillScreenDMA(volatile u16 color) {
  // TODO: IMPLEMENT
  UNUSED(color);

  DMA[3].src = &color;
  DMA[3].dst = videoBuffer;
  DMA[3].cnt = (WIDTH * HEIGHT) | DMA_ON | DMA_SOURCE_FIXED | DMA_DESTINATION_INCREMENT;
}

void drawChar(int row, int col, char ch, u16 color) {
  for (int i = 0; i < 6; i++) {
    for (int j = 0; j < 8; j++) {
      if (fontdata_6x8[OFFSET(j, i, 6) + ch * 48]) {
        setPixel(row + j, col + i, color);
      }
    }
  }
}

void drawString(int row, int col, char *str, u16 color) {
  while (*str) {
    drawChar(row, col, *str++, color);
    col += 6;
  }
}

void drawCenteredString(int row, int col, int width, int height, char *str, u16 color) {
  u32 len = 0;
  char *strCpy = str;
  while (*strCpy) {
    len++;
    strCpy++;
  }

  u32 strWidth = 6 * len;
  u32 strHeight = 8;

  int new_row = row + ((height - strHeight) >> 1);
  int new_col = col + ((width - strWidth) >> 1);
  drawString(new_row, new_col, str, color);
}

// additional functions required for program

void setupInitial(Player *player, Enemy enemy[]) {
  (*player).row = 70;
  (*player).col = 10;
  (*player).lives = 3;

  for(int i = 0; i < 5; i++) {
    enemy[i].row = 40 + (i * 25);
    enemy[i].col= 100;
    enemy[i].headRow = 2;
  }
}

void welcomeScreen(const u16 *image) {
	drawFullScreenImageDMA(image);
	drawCenteredString(WELCOMESCREEN_HEIGHT/2 + 45, WELCOMESCREEN_WIDTH/2 - 10, 2, 5, "Press ENTER to start", WHITE);
}

void instructionScreen(void) {
	drawString(10, 15, "INSTRUCTIONS:", RED);
	drawString(30, 15, "Reach the treasure to WIN", BLACK);
	drawString(50, 15, "You start with 3 lives", BLACK);
	drawString(70, 15, "LOSE 1 life if u hit green enemies", BLUE);
	drawString(90, 15, "Use arrow keys to MOVE red square", GREEN);
	drawString(110, 15, "Press BACKSPACE to restart anytime", BLACK);
	drawString(140, 15, "Hit ENTER to continue", RED);
}

void gameScreen(Player *player, Enemy enemy[], const u16 *image) {
	drawRectDMA((*player).row, (*player).col, 9, 9, RED);
	for(int i = 0; i < 5; i++) {
		drawRectDMA(enemy[i].row, enemy[i].col, 11, 11, GREEN);
	}
	drawImageDMA(HEIGHT - ENDSCREEN_HEIGHT - 10, WIDTH - ENDSCREEN_WIDTH, ENDSCREEN_WIDTH, ENDSCREEN_HEIGHT, image);
	drawString(150, 190, "Lives:", RED);
	drawChar(150, 230, (*player).lives + '0', RED);
}

int movePlayer(Player *player, volatile u32 buttons, Enemy enemy[]) {
	if(KEY_DOWN(BUTTON_RIGHT, buttons)) {
		drawRectDMA((*player).row, (*player).col, 9, 9, WHITE);
		if((*player).col < 232) {
			(*player).col += 1;
		}
	}
	if(KEY_DOWN(BUTTON_UP, buttons)) {
		drawRectDMA((*player).row, (*player).col, 9, 9, WHITE);
		if((*player).row > 0) {
			(*player).row -= 1;
		}
	}
	if(KEY_DOWN(BUTTON_LEFT, buttons)) {
		drawRectDMA((*player).row, (*player).col, 9, 9, WHITE);
		if((*player).col > 0) {
			(*player).col -= 1;
		}
	}
	if(KEY_DOWN(BUTTON_DOWN, buttons)) {
		drawRectDMA((*player).row, (*player).col, 9, 9, WHITE);
		if((*player).row < 152) {
			(*player).row += 1;
		}
	}
	drawRectDMA((*player).row, (*player).col, 9, 9, RED);
	for(int i = 0; i < 5; i++) {
		if((*player).row + 8 >= enemy[i].row && (*player).row <= enemy[i].row + 10 && (*player).col + 8 >= enemy[i].col && (*player).col <= enemy[i].col + 10) {
			(*player).lives--;
			if((*player).lives == 0) {
				return 2;
			} else {
				(*player).row = 5;
				(*player).col = 5;
				return 3;
			}
		}
	}

	if((*player).row + 8 >= HEIGHT - ENDSCREEN_HEIGHT - 10 && (*player).col + 8 >= WIDTH - ENDSCREEN_WIDTH) {
		return 1;
	}
	return 0;
}

void enemyMovement(Enemy enemy[]) {
	for(int i = 0; i < 5; i++) {
		drawRectDMA(enemy[i].row, enemy[i].col, 10, 10, WHITE);
	}
    if(enemy[4].row > 150) {
			for(int j = 0; j < 5; j++) {
				enemy[j].headRow = -enemy[j].headRow;
			}
		}
		if(enemy[0].row < 0) {
			for(int j = 0; j < 5; j++) {
				enemy[j].headRow = -enemy[j].headRow;
			}
		}
		
	for(int i = 0; i < 5; i++) {
		enemy[i].row += enemy[i].headRow;
		drawRectDMA(enemy[i].row, enemy[i].col, 10, 10, GREEN);
	}
}



  void endScreen(const u16 *image) {
	drawFullScreenImageDMA(image);
	drawCenteredString(WELCOMESCREEN_HEIGHT - 10, WELCOMESCREEN_WIDTH/2, 10, 10, "Press BACKSPACE to restart", RED);
}