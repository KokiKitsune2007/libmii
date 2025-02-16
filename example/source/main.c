#include <wiiuse/wpad.h>
#include <ogc/video.h>
#include <mii.h>
#include <stdio.h>
#include <stdlib.h>

static void *xfb = NULL;
static GXRModeObj *rmode = NULL;

char * MII_COLORS[12] = {"red", "orange", "yellow", "green", "light green", "blue", "light blue", "pink", "purple", "brown", "white", "black"};

int run = 1;

void init(){
	VIDEO_Init();
	WPAD_Init();
	rmode = VIDEO_GetPreferredMode(NULL);
	xfb = MEM_K0_TO_K1(SYS_AllocateFramebuffer(rmode));
	console_init(xfb,20,20,rmode->fbWidth,rmode->xfbHeight,rmode->fbWidth*VI_DISPLAY_PIX_SZ);
	VIDEO_Configure(rmode);
	VIDEO_SetNextFramebuffer(xfb);
	VIDEO_SetBlack(FALSE);
	VIDEO_Flush();
	VIDEO_WaitVSync();
	if(rmode->viTVMode&VI_NON_INTERLACE) VIDEO_WaitVSync();
	printf("\x1b[2;0H");
}


void showMii(Mii mii){
	printf("Press Left and Right on the Wii mote to navigate through your Miis\n\n");

	printf("Name: %s\n", mii.name);
	printf("By: %s\n", mii.creator);

	if (mii.female) printf("Gender: female\n");
	else printf("Gender: male\n");

	printf("Weight: %i\n", mii.weight);
	printf("Height: %i\n", mii.height);

	if (mii.favorite) printf("Favorite\n");

	if (mii.month>0 && mii.day>0)
		printf("Birthday: %i/%i\n", mii.month, mii.day);

	if (mii.downloaded) printf("Downloaded\n");

	printf("Favorite Color: %s\n", MII_COLORS[mii.favColor]);

	//printf("Skin: %i\n", mii.skinColor);
	//printf("Hair Color: %i\n", mii.hairColor);
	//printf("Eyebrow Color: %i\n", mii.eyebrowColor);
	//printf("Eye Color: %i\n", mii.eyeColor);
	//printf("Facial Hair Color: %i\n", mii.facialHairColor);
	//printf("Glases Color: %i\n", mii.glassesColor);*/
	//printf("Lip Color: %i\n", mii.lipColor);

	if (mii.mole) printf("Has mole\n");

	if (mii.invalid) printf("?\n");
}

void clearScreen(){printf("\033[2J");printf("\x1b[2;0H");}

void exitMii(){run = 0;}
void batterydead(s32 chan){printf("Wiimote %i battery is dead\n", chan);}

int main(){
	init();

	Mii * miis;

	//WPAD_SetBatteryDeadCallback(batterydead);
	//WPAD_SetPowerButtonCallback(exitMii);

	miis = loadMiis_Wii();

	int n = 0;

	showMii(miis[n]);

	while(run) {
		WPAD_ScanPads();
		u32 pressed = WPAD_ButtonsDown(0);
		if ((pressed & WPAD_BUTTON_RIGHT || pressed & WPAD_BUTTON_2 || pressed & WPAD_BUTTON_PLUS || pressed & WPAD_BUTTON_DOWN || pressed & WPAD_BUTTON_A) && n+1<100){
			clearScreen();
			n+=1;
			showMii(miis[n]);
		} else if ((pressed & WPAD_BUTTON_LEFT || pressed & WPAD_BUTTON_1 || pressed & WPAD_BUTTON_MINUS || pressed & WPAD_BUTTON_UP  || pressed & WPAD_BUTTON_B) && n>0) {
			clearScreen();
			n-=1;
			showMii(miis[n]);
		} else if (pressed & WPAD_BUTTON_HOME){
			clearScreen();
			printf("Goodbye!\n");
			run = 0;
		}
		VIDEO_WaitVSync();
	}
	return 0;
}
