#include "head.h"

extern u8 l_time_buf0[11];
extern u8 l_time_buf1[11];

extern time_t g_time;

extern u8 g_temp;
extern u8 g_goal_temp;

bit g_bink = FALSE;
void ShowMenu(void)
{	
	OLED_Clear();
	OLED_ShowString(0,4, "Now  Temp:   '");	
	OLED_ShowString(0,6, "Goal Temp:   '");

}

void ShowTemp(void)
{
	static u8 pre_temp = 0, pre_goal_temp = 0;
	if (g_temp != pre_temp) {
		
		OLED_ShowChar(84,4, g_temp / 10 + '0');	
		OLED_ShowChar(92,4, g_temp % 10 + '0');	
		
		pre_temp = g_temp;
	}
	
	if (g_goal_temp != pre_goal_temp) {
		
		OLED_ShowChar(84,6, g_goal_temp / 10 + '0');	
		OLED_ShowChar(92,6, g_goal_temp % 10 + '0');
		pre_goal_temp = g_goal_temp;
	}
}


static void __ShowBlink(enum blink mode, u8 en)
{
	u8 i = 0;
	if (en) {
		switch (mode) {
		case BLINK_NONE:
			Read_RTC_Char();
			break;
		case BLINK_YEAR:
			for (i = 0; i < 4; i++) {
				l_time_buf0[i] = ' ';
			}
			break;
		case BLINK_MON:
			for (i = 5; i < 7; i++) {
				l_time_buf0[i] = ' ';
			}
			break;
		case BLINK_DAY:
			for (i = 8; i < 10; i++) {
				l_time_buf0[i] = ' ';
			}
			break;
		case BLINK_HOUR:
			for (i = 0; i < 2; i++) {
				l_time_buf1[i] = ' ';
			}
			break;
		case BLINK_MIN:
			for (i = 3; i < 5; i++) {
				l_time_buf1[i] = ' ';
			}
			break;
		case BLINK_SEC:
			for (i = 6; i < 8; i++) {
				l_time_buf1[i] = ' ';
			}
			break;
		default:
			Read_RTC_Char();
			break;
		
		
		}
	} else {
		Read_RTC_Char();
	}
}
void ShowTimer (enum blink mode)
{
	static u8 setting;
	
	if (g_bink) {
		setting++;
		setting %= 2;
		
		__ShowBlink(mode, setting);

		OLED_ShowString(24,0, l_time_buf0);
		OLED_ShowString(32,2, l_time_buf1);
		
		g_bink = FALSE;
	}
}