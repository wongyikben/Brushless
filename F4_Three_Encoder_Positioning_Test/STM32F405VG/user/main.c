#include "main.h"
u16 mode = 0;
s32 MAX_VEL = 70000; 
s32 MAX_VEL_PRESC = 0;
u16 ACCEL_Fake = 800; //800
u16 ACCEL = 0;
u8 reverse = 0;
u16 i=0;
s32 PID_KP = 11410;
s32 PID_KI = 186;
s32 PID_KD = 74;

// 400 = 60  140
// 500 = 52  124
// 600 = 45  110
// 700 = 37  94
// 800 = 30  80
// 
//
u16 accel_count = 30;
u16 decel_count = 80;

s32 en1 = 0;
s32 en2 = 0;
s32 en3 = 0;
s32 old_en1 = 0;
s32 old_en2 = 0;
s32 old_en3 = 0;



u32 ticks_img = 0;
u16 base_dir = 0;
s32 base_raw_vel[3] = {0};

s16 joy_x, joy_y = 0;
s16 cur_x,cur_y = 0;
s16 joy_dir = 0;
s32 joy_vel = 0;

s32 joy_cur_vel = 0;


void motor_reset() {
	base_raw_vel[0] = 0;
	base_raw_vel[1] = 0;
	base_raw_vel[2] = 0;
}

void motor_translate(s16 dir, s32 vel) {
	base_raw_vel[0] += int_sin(dir * 10) * vel / 10000;
	base_raw_vel[1] += int_sin(dir * 10 + 1200) * vel / 10000;
	base_raw_vel[2] += int_sin(dir * 10 + 2400) * vel / 10000;
}

void motor_rotate(s32 vel) {
	base_raw_vel[0] += vel;
	base_raw_vel[1] += vel;
	base_raw_vel[2] += vel;
}

void motor_update() {
	if (mode ==0){
	MAX_VEL_PRESC = (MAX_VEL / 1000);
	ACCEL = (ACCEL_Fake/MAX_VEL_PRESC);
	joy_x = gar_get_joy(GAR_JOY_X);
	joy_y = gar_get_joy(GAR_JOY_Y);
	
	
/*	joy_dir = int_arc_tan2(joy_y, joy_x) + 270;
	if (joy_dir >= 360) joy_dir -= 360;
	if (!(joy_x || joy_y)) joy_dir = 0;
	
	joy_vel = Sqrt(joy_x * joy_x + joy_y * joy_y) * MAX_VEL_PRESC / 1024;
	
	if (joy_vel > joy_cur_vel) {
		joy_cur_vel += ACCEL;
		joy_cur_vel = (joy_cur_vel > joy_vel ? joy_vel : joy_cur_vel);
	} else if (joy_vel < joy_cur_vel) {
		joy_cur_vel -= ACCEL;
		joy_cur_vel = (joy_cur_vel < joy_vel ? joy_vel : joy_cur_vel);
	}
*/	
	// test
	if(joy_x>cur_x){
		cur_x+=ACCEL;
		cur_x = (cur_x > joy_x ? joy_x : cur_x);
	}else if(joy_x<cur_x){
		cur_x-=ACCEL;
		cur_x = (cur_x < joy_x ? joy_x : cur_x);
	}
	
	if(joy_y>cur_y){
		cur_y+=ACCEL;
		cur_y = (cur_y > joy_y ? joy_y : cur_y);
	}else if(joy_y<cur_y){
		cur_y-=ACCEL;
		cur_y = (cur_y < joy_y ? joy_y : cur_y);
	}
	
	joy_dir = int_arc_tan2(cur_y, cur_x) + 270;
	if (joy_dir >= 360) joy_dir -= 360;
	if (!(cur_x || cur_y)) joy_dir = 0;
	joy_vel = Sqrt(joy_x * joy_x + joy_y * joy_y)* MAX_VEL_PRESC / 1024 ;
	joy_cur_vel = Sqrt(cur_x * cur_x + cur_y * cur_y)* MAX_VEL_PRESC / 1024;
	
	motor_reset();
	gar_button_update();
	motor_translate(joy_dir, joy_cur_vel);
	if (gar_button_pressed(GAR_SW13))
		motor_rotate(MAX_VEL/10);
	if (gar_button_pressed(GAR_SW10))
		motor_rotate(-MAX_VEL/10);
	
	
	gun_can_motor_set_vel(MOTOR_1, base_raw_vel[0], CLOSE_LOOP);
	gun_can_motor_set_vel(MOTOR_2, base_raw_vel[1], CLOSE_LOOP);
	gun_can_motor_set_vel(MOTOR_3, base_raw_vel[2], CLOSE_LOOP);
	}
	else{
		MAX_VEL_PRESC = (3999 / 1000);
		ACCEL = (ACCEL_Fake/MAX_VEL_PRESC);
		cur_x = gar_get_joy(GAR_JOY_X);
		cur_y = gar_get_joy(GAR_JOY_Y);
		joy_x = cur_x;
		joy_y = cur_y;

		joy_dir = int_arc_tan2(cur_y, cur_x) + 270;
		if (joy_dir >= 360) joy_dir -= 360;
		if (!(cur_x || cur_y)) joy_dir = 0;
		joy_vel = Sqrt(joy_x * joy_x + joy_y * joy_y)* MAX_VEL_PRESC / 1024 ;
		joy_cur_vel = Sqrt(cur_x * cur_x + cur_y * cur_y)* MAX_VEL_PRESC / 1024;
	
	motor_reset();
	motor_translate(joy_dir, joy_cur_vel);
	if (gar_button_pressed(GAR_SW13))
		motor_rotate(1000);
	if (gar_button_pressed(GAR_SW10))
		motor_rotate(-1000);
	gun_can_motor_set_vel(MOTOR_1, base_raw_vel[0], OPEN_LOOP);
	gun_can_motor_set_vel(MOTOR_2, base_raw_vel[1], OPEN_LOOP);
	gun_can_motor_set_vel(MOTOR_3, base_raw_vel[2], OPEN_LOOP);
	
	
	}
}

void motor_update_debug(u8 dirr) {
	if (mode ==0){
	MAX_VEL_PRESC = (MAX_VEL / 1000);
	ACCEL = (ACCEL_Fake/MAX_VEL_PRESC);
	joy_x = 0;
	joy_y = 1000;
	if(dirr==3){
			joy_x = 0;
			joy_y = 0;
	}
	if(reverse == 1)
		joy_y *= -1;
	
/*	joy_dir = int_arc_tan2(joy_y, joy_x) + 270;
	if (joy_dir >= 360) joy_dir -= 360;
	if (!(joy_x || joy_y)) joy_dir = 0;
	
	joy_vel = Sqrt(joy_x * joy_x + joy_y * joy_y) * MAX_VEL_PRESC / 1024;
	
	if (joy_vel > joy_cur_vel) {
		joy_cur_vel += ACCEL;
		joy_cur_vel = (joy_cur_vel > joy_vel ? joy_vel : joy_cur_vel);
	} else if (joy_vel < joy_cur_vel) {
		joy_cur_vel -= ACCEL;
		joy_cur_vel = (joy_cur_vel < joy_vel ? joy_vel : joy_cur_vel);
	}
*/	
	// test
	if(joy_x>cur_x){
		cur_x+=ACCEL;
		cur_x = (cur_x > joy_x ? joy_x : cur_x);
	}else if(joy_x<cur_x){
		cur_x-=ACCEL;
		cur_x = (cur_x < joy_x ? joy_x : cur_x);
	}
	
	if(joy_y>cur_y){
		cur_y+=ACCEL;
		cur_y = (cur_y > joy_y ? joy_y : cur_y);
	}else if(joy_y<cur_y){
		cur_y-=ACCEL;
		cur_y = (cur_y < joy_y ? joy_y : cur_y);
	}
	
	//joy_dir = int_arc_tan2(cur_y, cur_x) + 270;
	//if (joy_dir >= 360) joy_dir -= 360;
	//if (!(cur_x || cur_y)) joy_dir = 0;
	joy_vel = Sqrt(joy_x * joy_x + joy_y * joy_y)* MAX_VEL_PRESC / 1024 ;
	joy_cur_vel = cur_y* MAX_VEL_PRESC;
	
	if(dirr == 0)
		joy_dir = 0;
	if(dirr == 1)
		joy_dir = 2400;
	if(dirr == 2)
		joy_dir = 1200;
	
	
	motor_reset();
	gar_button_update();
	motor_translate(joy_dir, joy_cur_vel);
	if (gar_button_pressed(GAR_SW13))
		motor_rotate(MAX_VEL/5);
	if (gar_button_pressed(GAR_SW10))
		motor_rotate(-MAX_VEL/5);
	
	
	gun_can_motor_set_vel(MOTOR_1, base_raw_vel[0], CLOSE_LOOP);
	gun_can_motor_set_vel(MOTOR_2, base_raw_vel[1], CLOSE_LOOP);
	gun_can_motor_set_vel(MOTOR_3, base_raw_vel[2], CLOSE_LOOP);
	}
	else{
		MAX_VEL_PRESC = (3999 / 1000);
		ACCEL = (ACCEL_Fake/MAX_VEL_PRESC);
		cur_x = gar_get_joy(GAR_JOY_X);
		cur_y = gar_get_joy(GAR_JOY_Y);
		joy_x = cur_x;
		joy_y = cur_y;

		joy_dir = int_arc_tan2(cur_y, cur_x) + 270;
		if (joy_dir >= 360) joy_dir -= 360;
		if (!(cur_x || cur_y)) joy_dir = 0;
		joy_vel = Sqrt(joy_x * joy_x + joy_y * joy_y)* MAX_VEL_PRESC / 1024 ;
		joy_cur_vel = Sqrt(cur_x * cur_x + cur_y * cur_y)* MAX_VEL_PRESC / 1024;
	
	motor_reset();
	motor_translate(joy_dir, joy_cur_vel);
	if (gar_button_pressed(GAR_SW13))
		motor_rotate(1000);
	if (gar_button_pressed(GAR_SW10))
		motor_rotate(-1000);
	gun_can_motor_set_vel(MOTOR_1, base_raw_vel[0], OPEN_LOOP);
	gun_can_motor_set_vel(MOTOR_2, base_raw_vel[1], OPEN_LOOP);
	gun_can_motor_set_vel(MOTOR_3, base_raw_vel[2], OPEN_LOOP);
	
	
	}
}






void debugOpen(){
	i=0;
	uart_tx(COM2,"OPEN LOOP Test \n");

	en1 = 0;
	en2 = 0;
	en3 = 0;
	
	old_en1 = 0;
	old_en2 = 0;
	old_en3 = 0;
	while(i<=72){
	if (get_ticks() != ticks_img) {
			ticks_img = get_ticks();
			if(ticks_img%1000 ==0 && i<=72){
					base_raw_vel[0] = ((i+1)*100)-1;
					base_raw_vel[1] = ((i+1)*100)-1;
					base_raw_vel[2] = ((i+1)*100)-1;
					
					
					en1=(gun_can_get_encoder(MOTOR_1));
					en2=(gun_can_get_encoder(MOTOR_2));
					en3=(gun_can_get_encoder(MOTOR_3));	

					gun_can_motor_set_vel(MOTOR_1, base_raw_vel[0], OPEN_LOOP);
					gun_can_motor_set_vel(MOTOR_2, base_raw_vel[1], OPEN_LOOP);
					gun_can_motor_set_vel(MOTOR_3, base_raw_vel[2], OPEN_LOOP);
					
					
					uart_tx(COM2,"%d %d %d %d;",base_raw_vel[0]-99,abs(en1-old_en1),abs(en2-old_en2),abs(en3-old_en3));
					old_en1 = en1;
					old_en2 = en2;
					old_en3 = en3;
					++i;
				
			}

	}
	}
					gun_can_motor_set_vel(MOTOR_1, 0, OPEN_LOOP);
					gun_can_motor_set_vel(MOTOR_2, 0, OPEN_LOOP);
					gun_can_motor_set_vel(MOTOR_3, 0, OPEN_LOOP);
	while(1){}
}

void debug(u8 dirr){
	i=0;
	if(reverse == 0 ){uart_tx(COM2," Mode %d %d\n [",dirr+1,ACCEL_Fake);}
	en1 = 0;
	en2 = 0;
	en3 = 0;
	
	old_en1 = 0;
	old_en2 = 0;
	old_en3 = 0;
	
	accel_count = -ACCEL_Fake*75/1000+90;
	decel_count = -ACCEL_Fake*15/100 + 200;
	
	while(i<decel_count){
		// Mode 1 treat Motor 1 as Front
		// Mode 2 treat Motor 2 as Front 
		// Mode 3 treat Motor 3 as Front
		if (get_ticks() != ticks_img) {
			ticks_img = get_ticks();
		
			if(ticks_img%10==0 && i<accel_count){
				motor_update_debug(dirr);
				
				en1=(gun_can_get_encoder(MOTOR_1));
				en2=(gun_can_get_encoder(MOTOR_2));
				en3=(gun_can_get_encoder(MOTOR_3));
				
				uart_tx(COM2,"%d %d %d %d %d %d %d %d %d;",base_raw_vel[0],base_raw_vel[1],base_raw_vel[2],
				gun_can_get_pwm(MOTOR_1),gun_can_get_pwm(MOTOR_2),gun_can_get_pwm(MOTOR_3),(en1-old_en1)*100-base_raw_vel[0],(en2-old_en2)*100-base_raw_vel[1],
				(en3-old_en3)*100-base_raw_vel[2]); // +ve = faster  -ve = slower
				old_en1=en1;
				old_en2=en2;
				old_en3=en3;
				i++;
			}
			else if(ticks_img%10==0&&i<decel_count){
				motor_update_debug(3);
				en1=(gun_can_get_encoder(MOTOR_1));
				en2=(gun_can_get_encoder(MOTOR_2));
				en3=(gun_can_get_encoder(MOTOR_3));
				
				uart_tx(COM2,"%d %d %d %d %d %d %d %d %d;",base_raw_vel[0],base_raw_vel[1],base_raw_vel[2],
				gun_can_get_pwm(MOTOR_1),gun_can_get_pwm(MOTOR_2),gun_can_get_pwm(MOTOR_3),(en1-old_en1)*100-base_raw_vel[0],(en2-old_en2)*100-base_raw_vel[1],
				(en3-old_en3)*100-base_raw_vel[2]); // +ve = faster  -ve = slower
				old_en1=en1;
				old_en2=en2;
				old_en3=en3;
				i++; 
			}
		}
	}
	if(reverse == 1){uart_tx(COM2,"]; \n");}
}








int main(void) {
	SystemInit();
	SystemCoreClockUpdate();
	gpio_rcc_init_all();
	uart_init(COM2, 115200);
	ticks_init();
	adc_init();

	tft_init((TFT_ORIENTATION)2, BLACK, WHITE, RED);
	led_init();
	buzzer_init();
	btn_init();
	timer_init();

	can_init();
	can_rx_init();
	gun_can_motor_init();
	garuda_init(GAR_SPI_FIRST);

	
	while (get_ticks() < 100);
	
	while(1) {
		if (get_ticks() != ticks_img) {
			ticks_img = get_ticks();
			
			if(ticks_img%30==2){
				if(gar_button_pressed(GAR_SHOOT)){
					gun_can_motor_set_vel(MOTOR_1, 0, OPEN_LOOP);
					gun_can_motor_set_vel(MOTOR_2, 0, OPEN_LOOP);
					gun_can_motor_set_vel(MOTOR_3, 0, OPEN_LOOP);
					tft_clear();
					tft_println("LOCK");
					tft_update();
					cur_x=0;
					cur_y=0;
					while(ticks_img>get_ticks()-2000){}
				
					}			
			}
				
			if(ticks_img%100==1){
				if (gar_button_pressed(GAR_POLE1)){
					debug(0);
					reverse = 1;
					debug(0);
					reverse = 0;
				}
					
				if (gar_button_pressed(GAR_POLE2)){
					debug(1);
					reverse = 1;
					debug(1);
					reverse = 0;}
				if (gar_button_pressed(GAR_POLE3)){
					debug(2);
					reverse = 1;
					debug(2);
					reverse = 0;}
				if (gar_button_pressed(GAR_IO6)){
						gun_set_PID(MOTOR_1,6);
						PID_KD+=5;
				}
					//ACCEL_Fake+=50;
				if (gar_button_pressed(GAR_IO5)){
						gun_set_PID(MOTOR_1,5);
						PID_KD-=5;
				}
					//ACCEL_Fake-=50;
			
			if(gar_button_pressed(GAR_POLE4))
				debugOpen();				
 			if (gar_button_pressed(GAR_IO3)){
				gun_set_PID(MOTOR_1,4);
				PID_KI+=5;
			}
				//	accel_count+=2;
			if (gar_button_pressed(GAR_IO4)){
				gun_set_PID(MOTOR_1,3);
				PID_KI-=5;
			}
				//	accel_count-=2;
			if (gar_button_pressed(GAR_IO2)){
				//	decel_count+=2;
				gun_set_PID(MOTOR_1,2);
				PID_KP+=5;}
			if (gar_button_pressed(GAR_IO1)){
				gun_set_PID(MOTOR_1,1);
				PID_KP-=5;
			}
				//	decel_count-=2;
				gar_button_update();
			}
			
			if(ticks_img % 10 ==3){
					motor_update();
					gar_button_update();
			}
			
			if (ticks_img % 50 == 0) {
				
				tft_clear();
				tft_println("Time:%d" ,ticks_img);		// time
				//tft_println("%d %d %d" ,ACCEL_Fake,accel_count,decel_count);
				//tft_println("%d", joy_dir); // joystick angle
				//tft_println("%d %d", joy_x, joy_y); //current velocity and target velocity 
				//tft_println("%d %d", joy_cur_vel, joy_vel);
				//tft_println("Vel 1: %d", base_raw_vel[0]); // MOTOR 1
				//tft_println("Vel 2: %d", base_raw_vel[1]); // MOTOR 2
				//tft_println("Vel 3: %d", base_raw_vel[2]); // MOTOR 3 
				//tft_println("PWM 1: %d", gun_can_get_pwm(MOTOR_1));
				//tft_println("PWM 2: %d", gun_can_get_pwm(MOTOR_2));
				//tft_println("PWM 3: %d", gun_can_get_pwm(MOTOR_3));
				
				tft_println("%d", PID_KP);
				tft_println("%d", PID_KI);
				tft_println("%d", PID_KD);
				
				
				tft_update();
			
			}
		}
	}
}
