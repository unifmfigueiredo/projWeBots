#include <stdio.h>
#include <webots/robot.h>
#include <webots/motor.h>
#include <webots/distance_sensor.h>
#include <webots/led.h>

#define TIME_STEP 64
#define SPEED 6.28
#define QtddSensoresProx 8

int main(int argc, char **argv){
	int cont = 0;
	int endCont = 15;
	double velocidadeEsquerda = 0;
	double 	velocidadeDireita = 0;
	bool passouPrimeiraDireita = false;
	bool paredeEsquerda = false;
	bool cantoEsquerdo = false;
	bool paredeFrontal = false;
	bool primeiraDireita = false;

	if (TIME_STEP == 128){
		endCont = 8;
	}

	wb_robot_init();

	// configurei MOTORES
	WbDeviceTag MotorEsquerdo, MotorDireito;

	MotorEsquerdo = wb_robot_get_device("left wheel motor");
	MotorDireito = wb_robot_get_device("right wheel motor");
	wb_motor_set_position(MotorEsquerdo, INFINITY);
	wb_motor_set_position(MotorDireito, INFINITY);
	wb_motor_set_velocity(MotorEsquerdo, 0);
	wb_motor_set_velocity(MotorDireito, 0);

	// configurei Sensores de Proximidade
	WbDeviceTag SensorProx[QtddSensoresProx];
	SensorProx[0] = wb_robot_get_device("ps0");
	wb_distance_sensor_enable(SensorProx[0], TIME_STEP);
	SensorProx[1] = wb_robot_get_device("ps1");
	wb_distance_sensor_enable(SensorProx[1], TIME_STEP);
	SensorProx[2] = wb_robot_get_device("ps2");
	wb_distance_sensor_enable(SensorProx[2], TIME_STEP);
	SensorProx[3] = wb_robot_get_device("ps3");
	wb_distance_sensor_enable(SensorProx[3], TIME_STEP);
	SensorProx[4] = wb_robot_get_device("ps4");
	wb_distance_sensor_enable(SensorProx[4], TIME_STEP);
	SensorProx[5] = wb_robot_get_device("ps5");
	wb_distance_sensor_enable(SensorProx[5], TIME_STEP);
	SensorProx[6] = wb_robot_get_device("ps6");
	wb_distance_sensor_enable(SensorProx[6], TIME_STEP);
	SensorProx[7] = wb_robot_get_device("ps7");
	wb_distance_sensor_enable(SensorProx[7], TIME_STEP);

	while (wb_robot_step(TIME_STEP) != -1){
		for (int i = 0; i < QtddSensoresProx; i++){
			printf("sensor %d: %5.2f", i, wb_distance_sensor_get_value(SensorProx[i]));
		}

		paredeEsquerda = wb_distance_sensor_get_value(SensorProx[5]) > 80;
		cantoEsquerdo = wb_distance_sensor_get_value(SensorProx[6]) > 80;
		paredeFrontal = wb_distance_sensor_get_value(SensorProx[7]) > 80;
		primeiraDireita = wb_distance_sensor_get_value(SensorProx[2]) > 80;

		if(passouPrimeiraDireita){
			primeiraDireita = false;
		}

		velocidadeEsquerda = SPEED;
		velocidadeDireita = SPEED;

		if(paredeFrontal){
			velocidadeEsquerda = SPEED;
			velocidadeDireita = -(SPEED);
		}else{
			if(paredeEsquerda){
				velocidadeEsquerda = SPEED;
				velocidadeDireita = SPEED;
			}else{
				velocidadeEsquerda = SPEED/8;
				velocidadeDireita = SPEED;
			}
			if((cantoEsquerdo) || (primeiraDireita)){
				if(primeiraDireita){
					cont = cont + 1;
					if(cont >= endCont){
						passouPrimeiraDireita = true;
					}
				}
				velocidadeEsquerda = SPEED;
				velocidadeDireita = SPEED/8;
			}
		}

		wb_motor_set_velocity(MotorEsquerdo, velocidadeEsquerda);
		wb_motor_set_velocity(MotorDireito, velocidadeDireita);
	};

	wb_robot_cleanup();
	return 0;
}
