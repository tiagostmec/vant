// SISTEMAS //
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <math.h>
#include <algorithm>
#include <ctime>
#include <string.h>
#include <sys/stat.h>
#include <time.h>

#define LOGNAME_FORMAT "%d-%b-%y_%H:%M:%S"
#define LOGNAME_SIZE 20
//////////////

//  MATRICIAL  //
#include <Eigen/Core>
#include <Eigen/Geometry>
#include <Eigen/LU>

/// ROS //////
#include <ros/ros.h>
#include <ar_track_alvar_msgs/AlvarMarkers.h>
#include <tf/transform_datatypes.h>
#include <geometry_msgs/Quaternion.h>
#include <geometry_msgs/PoseStamped.h>
#include <geometry_msgs/TwistStamped.h>
#include <tf/transform_listener.h>
//////////////
//#include <vant/vaant.h> //ANTES
#include "/home/laic/catkin_ws/src/vant/include/vant/vaant.h" //RODRIGO ALTEROU ISSO AQUI
#include <vant/vision.h>
//#include <vant/ToOffboard.h> //ANTES
#include "/home/laic/catkin_ws/src/vant/include/vant/ToOffboard.h" //RODRIGO ALTEROU ISSO AQUI

/// FUZZY ////
//#include "fl/Headers.h" //Usado para fuzzy tipo 1
//////////////

////SkyPat////
#include <unistd.h>
#include </home/laic/catkin_ws/src/vant/include/skypat/skypat/skypat.h>
//#include </home/laic/catkin_ws/src/vant/include/skypat/Path.h>
//#include </home/laic/catkin_ws/src/vant/include/skypat/Thread/Thread.h>
//#include </home/laic/catkin_ws/src/vant/include/skypat/Config.h>
//#include </home/laic/catkin_ws/src/vant/include/skypat/Perf.h>
//#include </home/laic/catkin_ws/src/vant/include/skypat/Timer.h>


//-------------------------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------------------------
//--------------------- CODIGO DE PERFORMANCE USANDO SKYPAT - PID VS FUZZY T2----------------------------------------------------------------
// -------------------------------Rodrigo Perobeli Silva Costa-------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------------------------


using namespace std;

//--------------------------------------------------------------------------------------------------------------------------------------------
// Variáveis de operação do VANT
double H_MAX = 5.0,                                             // altura maxima de operacao (metros)
       ABERTURA_DA_CAMERA_X = DEG2RAD(45.0),                    // Eixo X da CAMERA! (Graus)
       ABERTURA_DA_CAMERA_Y = DEG2RAD(36.0),                    // Eixo Y da CAMERA! (Graus)
       MAX_X = H_MAX*tan(ABERTURA_DA_CAMERA_Y),                 // Eixo X DO VANT !
       MAX_Y = H_MAX*tan(ABERTURA_DA_CAMERA_X),                 // Eixo Y do VANT !
			 tolZ = 0.5;																							// Limite para pousar (metros) 
//-------------------------------------------------------------------------------------------------------------------------------------------
// variáveis para Calcula_PID
double kp = 1, //constante proporcional
       ki = 1, //constante integrativa
       kd = 0; //constante derivativa

double tempX=0, tempY=0, tempZ=0;   //Variáveis temporárias para posição X Y e Z a cada iteração
double tempXprev=0, tempYprev=0, tempZprev=0; //Posições anteriores auxiliares ao calculo derivativo

double f1=0,f2=0; //variaveis para somar

////////////////////////////////////////////////////////////// INICIO PID //////////////////////////////////////////////////////////////////

double Calcula_PID(double erro, double prev_erro)
    {
        double i=0,p=0,d=0,v = 0;

        p = kp*erro;

        if (abs(erro) < 0.5)
            {
                i = i + ki*erro;
            }

        d = kd*(erro-prev_erro);

        v = p+i+d;
        return(v);
    }

////////////////////////////////////////////////////////////// FIM PID //////////////////////////////////////////////////////////////////

//Soma 2 valores
double soma(double fator1, double fator2)
{
	double resp = fator1+fator2;
	return resp;
}

///////////////////////////////////////////////////////////INICIO SKYPAT ///////////////////////////////////////////////////
/*SKYPAT_F(PID_Case, pid_test)
{
  PERFORM(skypat::CONTEXT_SWITCHES)
  {
    Calcula_PID(tempX,tempXprev);
    Calcula_PID(tempY,tempYprev);
    Calcula_PID(tempZ,tempZprev);
      
  }
  PERFORM(skypat::CPU_CLOCK)
  {
    Calcula_PID(tempX,tempXprev);
    Calcula_PID(tempY,tempYprev);
    Calcula_PID(tempZ,tempZprev);
      
  }
  PERFORM(skypat::TASK_CLOCK)
  {
    Calcula_PID(tempX,tempXprev);
    Calcula_PID(tempY,tempYprev);
    Calcula_PID(tempZ,tempZprev);
      
  }
  PERFORM(skypat::CPU_CYCLES)
  {
    Calcula_PID(tempX,tempXprev);
    Calcula_PID(tempY,tempYprev);
    Calcula_PID(tempZ,tempZprev);
      
  }
  PERFORM(skypat::INSTRUCTIONS)
  {
    Calcula_PID(tempX,tempXprev);
    Calcula_PID(tempY,tempYprev);
    Calcula_PID(tempZ,tempZprev);
      
  }
  PERFORM(skypat::CACHE_REFERENCES)
  {
    Calcula_PID(tempX,tempXprev);
    Calcula_PID(tempY,tempYprev);
    Calcula_PID(tempZ,tempZprev);
      
  }
  PERFORM(skypat::CPU_CYCLES)
  {
    Calcula_PID(tempX,tempXprev);
    Calcula_PID(tempY,tempYprev);
    Calcula_PID(tempZ,tempZprev);
      
  }
  PERFORM(skypat::REF_CPU_CYCLES)
  {
    Calcula_PID(tempX,tempXprev);
    Calcula_PID(tempY,tempYprev);
    Calcula_PID(tempZ,tempZprev);
  }
}


SKYPAT_F(PID_Case, pid_test)
{
	PERFORM(skypat::CONTEXT_SWITCHES)
  {
    soma(f1,f2);      
  }
}*/
////////////////////////////////////////////////////////////FIM SKYPAT ////////////////////////////////////////////////////

int main(int argc, char* argv[])
{
		/*// ENTRADA EM PORCENTAGEM, OU SEJA, ESCALA DE -100 A 100
    if(argc == 1){

    }
    else{
    tempX = atof(argv[1]);
    tempY = atof(argv[2]);
    tempZ = atof(argv[3]);
    }

    cout << "{x,y,z}: "<< tempX << " " << tempY << " " << tempZ << endl;

    
    vector<double> v(3);
   
    timeval oldCount,newCount;
    gettimeofday(&oldCount, NULL);

    gettimeofday(&newCount, NULL);
    double t = double(newCount.tv_sec -oldCount.tv_sec )
            + double(newCount.tv_usec-oldCount.tv_usec) * 1.e-6;// tava 1e-6
    

    gettimeofday(&oldCount, NULL);

    //v = fuzzy(tx,ty);

    gettimeofday(&newCount, NULL);
    t = double(newCount.tv_sec -oldCount.tv_sec )
            + double(newCount.tv_usec-oldCount.tv_usec) * 1.e-6;// tava 1e-6

    cout << "Tempo Fuzzy: "<< t << endl;*/

		//codigo teste de soma
		if(argc ==1){}
		else
		{
			f1 = atof(argv[1]);
			f2 = atof(argv[2]);
		}
    //skypat::Test::Initialize(argc, argv);
    //skypat::Test::RunAll();

}

