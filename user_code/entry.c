#include "head.h"

extern time_t g_time;
extern u8 KeyValue;

extern u8 g_rtc[6];
extern unsigned char l_timer[7];

static u8 g_rtc_mode = BLINK_NONE;

u8 g_adjust = 0;

u8 g_temp      = 0;
u8 g_goal_temp = 30;

static char _char_limit(char min, char max, char now)
{
	if (now < min) {
		now = max;
	} else if (now > max){
		now = min;
	}
	return now;
}

static u8 _u8_limit (u8 min, u8 max, u8 now)
{
	if (now < min) {
		now = min;
	} else if (now > max){
		now = max;
	}
	return now;
}

static void data_update(void)
{
	float temp = 0;
	u8 a;
	if (g_time.time_1000ms) {
		
		if (g_rtc_mode == 0) {
			Read_RTC();
		}
		temp = (float)TMP() / 10;
		
		g_temp = (u8)(temp);
		
		if (led_pin == 0) {
			g_adjust = pid((float)g_goal_temp, temp);
			g_adjust = _u8_limit (0, 100, g_adjust);
		} else {
			g_adjust = 0;
			HEAT_OFF();
			
// 			UART1_SendData(a / 100 + '0');
// 			UART1_SendData(a / 10 %10 + '0');
// 			UART1_SendData(a % 10 + '0');
// 			UART1_SendData(0X0A);
		}
		a = g_adjust;

		UART1_SendData(l_timer[6]);
		UART1_SendData(g_rtc[0]);
		g_time.time_1000ms = 0;
	}
	
	if (KeyValue == 0x04) {
		led_pin = ~led_pin;
	}
}


static void rtc_set(void)
{
	bit rtc_set = FALSE;
	
	char val = 0;
 	if (KeyValue == 0x08) {
		g_rtc_mode ++;
		
		g_rtc_mode %= BLINK_NUM;
	}
	
	if (g_rtc_mode) {
		if (KeyValue == 0x01) {
			val = 1;
			rtc_set = TRUE;
		} else if (KeyValue == 0x02){
			val = -1;
			rtc_set = TRUE;
		}
	}

	
	switch (g_rtc_mode) {
	case BLINK_NONE:
		break;
	case BLINK_YEAR:
		g_rtc[g_rtc_mode - 1] = _char_limit(0, 100, (g_rtc[g_rtc_mode - 1] + val));
		break;
	case BLINK_MON:
		g_rtc[g_rtc_mode - 1] = _char_limit(0, 12, (g_rtc[g_rtc_mode - 1] + val));
		break;
	case BLINK_DAY:
		g_rtc[g_rtc_mode - 1] = _char_limit(0, 31, (g_rtc[g_rtc_mode - 1] + val));
		break;
	case BLINK_HOUR:
		g_rtc[g_rtc_mode - 1] = _char_limit(0, 24, (g_rtc[g_rtc_mode - 1] + val));
		break;
	case BLINK_MIN:
		g_rtc[g_rtc_mode - 1] = _char_limit(0, 60, (g_rtc[g_rtc_mode - 1] + val));
		break;
	case BLINK_SEC:
		g_rtc[g_rtc_mode - 1] = _char_limit(0, 60, (g_rtc[g_rtc_mode - 1] + val));		
		break;
	default:
		break;
	}
	
	if (rtc_set) {
		Set_RTC();
    }
	ShowTimer (g_rtc_mode);
}

static void goal_temp_set(void)
{
	if (g_rtc_mode == 0)
	{
		if (led_pin == 1) {
			if (KeyValue == 0x01) {
				g_goal_temp++;
			} else if (KeyValue == 0x02){
				g_goal_temp--;
			}
		}
	}
}

/********************************************************** 
*��������
*���ܣ� ��������
*����ֵ��
�� ע�� 
**********************************************************/
void entry(void)
{
	ReadKey();
	
	if (KeyValue) {
		UART1_SendData(KeyValue);
	}
	
	
	data_update();
	rtc_set();
	goal_temp_set();
	ShowTemp();
	
	KeyValue = 0;
	
}