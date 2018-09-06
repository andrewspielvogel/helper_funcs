/**
 * @file
 * @date May 2016
 * @author Andrew Spielvogel (andrewspielvogel@gmail.com)
 * @brief Helper functions
 *
 */

#include <fstream>
#include <math.h>
#include <stdio.h>
#include <Eigen/Core>
#include <helper_funcs/helper_funcs.h>
#include <unsupported/Eigen/MatrixFunctions>
#include <iostream>



/**
 * @brief Skew function.
 * 
 * Skew operator on \f${\rm I\!R}^3\f$.
 * @param w Input vector.
 */
Eigen::Matrix3d skew(Eigen::Vector3d w)
{

  Eigen::Matrix3d w_hat;
  w_hat << 0.0,-w(2),w(1),w(2),0.0,-w(0),-w(1),w(0),0.0;
  
  return w_hat;

}

/**
 * @brief Unskew function.
 *
 * @param w_hat Input so(3) matrix.
 */
Eigen::Vector3d unskew(Eigen::Matrix3d w_hat)
{

  Eigen::Vector3d w(-w_hat(1,2),w_hat(0,2),-w_hat(0,1));

  return w;

}

/**
 * @brief Rotation around x-axis.
 * @param x angle (units: radians).
 */
Eigen::Matrix3d Rx(double x)
{

  Eigen::Matrix3d Rx;
  Rx << 1,0,0,0,cos(x),-sin(x),0,sin(x),cos(x);

  return Rx;

}

/**
 * @brief Rotation around y-axis.
 * @param y angle (units: radians).
 */
Eigen::Matrix3d Ry(double y)
{

  Eigen::Matrix3d Ry;
  Ry << cos(y),0,sin(y),0,1,0,-sin(y),0,cos(y);

  return Ry;

}

/**
 * @brief Rotation around z-axis.
 * @param z angle (units: radians).
 */
Eigen::Matrix3d Rz(double z)
{

  Eigen::Matrix3d Rz;
  Rz << cos(z),-sin(z),0,sin(z),cos(z),0,0,0,1;

  return Rz;

}

/**
 * @brief Rotation to roll, pitch, heading euler angles.
 * @param R Input rotation.
 */
Eigen::Vector3d rot2rph(Eigen::Matrix3d R)
{

  double h = atan2(R(1,0),R(0,0));
  
  double ch = cos(h);
  double sh = sin(h);
  double p = atan2(-R(2,0), R(0,0)*ch + R(1,0)*sh);
  double r = atan2(R(0,2)*sh - R(1,2)*ch, -R(0,1)*sh + R(1,1)*ch);

  Eigen::Vector3d rph(r,p,h);

  return rph;

}

/**
 * @brief Roll, pitch, yaw euler angles to rotation.
 * @param rpy Roll, pitch, yaw vector (units: radians).
 */

Eigen::Matrix3d rpy2rot(Eigen::Vector3d rpy)
{

  return Rz(rpy(2))*Ry(rpy(1))*Rx(rpy(0));
  
}

/**
 * @brief Earth to NED rotation.
 * @param lat Latitude of instrument.
 */
Eigen::Matrix3d rot_earth2ned(float lat)
{

  Eigen::Matrix3d R;
  R << -sin(lat),0,-cos(lat),0,1,0,cos(lat),0,-sin(lat);
  return R;

}


/**
 * @brief Returns Star to Earth rotation
 * @param t Time (seconds)
 */
Eigen::Matrix3d get_R_se(float t)
{

  float rate = 15.041*M_PI/180/3600;

  Eigen::Vector3d w(0,0,1.0);

  Eigen::Matrix3d w_hat = skew(w)*rate*t;

  Eigen::Matrix3d R_se = w_hat.exp();

  return R_se;
}

/**
 * @brief Returns Earth to NED rotation
 * @param lat Latitude (radians)
 */
Eigen::Matrix3d get_R_en(float lat)
{

  Eigen::Matrix3d R_en;

  R_en << -sin(lat),0,-cos(lat),0,1,0,cos(lat),0,-sin(lat);

  return R_en;

}

/**
 * @brief Returns Star to NED rotation
 * @param lat Latitude (radians)
 * @param t Time (seconds)
 */
Eigen::Matrix3d get_R_sn(float lat, float t)
{

  Eigen::Matrix3d R_en = get_R_en(lat);
  
  Eigen::Matrix3d R_se = get_R_se(t);

  Eigen::Matrix3d R_sn = R_se*R_en;

  return R_sn;


}


/**
 *
 * @brief Function for converting string to 3x3 diagonal matrix.
 *
 * @param str Comma seperated list of three diag elements. e.g. "1,1,1"
 *
 */
Eigen::Matrix3d stringToDiag(std::string str)
{
  Eigen::Matrix3d diag;
  Eigen::Vector3d vec;
  sscanf(str.c_str(),"%*[[]%lf,%lf,%lf%*[]]",&vec(0),&vec(1),&vec(2));

  diag << vec(0),0,0,0,vec(1),0,0,0,vec(2);

  return diag;

}


/**
 *
 * @brief Function for loading parameters from config_file.
 *
 * @param Config file to parse.
 *
 */
config_params load_params(char* config_file)
{

  printf("LOADING CONFIG FILE: %s\n",config_file);
  
  config_params params;
  std::ifstream infile(config_file);
  std::string line;

  /**********************************
   *  LOAD IN LINE FROM CONFIG FILE
   **********************************/
  while (std::getline(infile, line))
  {

    char field[128] = "";
    char data[128]  = "";

    /*********************************
     *          PARSE LINE
     *********************************/
    sscanf(line.c_str(),"%s = %s",field,data);
    
    if((std::string(field))=="hz")
    {

      sscanf(data,"%d",&params.hz);

    }
    else if((std::string(field))=="lat")
    {

      sscanf(data,"%f",&params.lat);
      params.lat = params.lat*M_PI/180.0;

    }
    else if((std::string(field))=="o_file")
    {

      char str[128];
      sscanf(data,"%s",str);
      params.o_file = std::string(str);
      params.o_file.erase(remove(params.o_file.begin(),params.o_file.end(), '\"' ),params.o_file.end());

    }
    else if((std::string(field))=="i_file")
    {
	
      char str[128];
      sscanf(data,"%s",str);
      params.i_file = std::string(str);
      params.i_file.erase(remove(params.i_file.begin(),params.i_file.end(), '\"' ),params.i_file.end());
      
    }
    else if((std::string(field))=="k_acc")
    {

      params.K_acc = stringToDiag(data);

    }
    else if((std::string(field))=="k_mag")
    {

      params.K_mag = stringToDiag(data);

    }
    else if((std::string(field))=="k_ang_bias")
    {

      params.K_ang_bias = stringToDiag(data);

    }
    else if((std::string(field))=="k_acc_bias")
    {

      params.K_acc_bias = stringToDiag(data);

    }
    else if((std::string(field))=="k_mag_bias")
    {

      params.K_mag_bias = stringToDiag(data);

    }
    else if((std::string(field))=="k_E_n")
    {

      params.K_E_n = stringToDiag(data);

    }
    else if((std::string(field))=="rpy_align")
    {
      Eigen::Vector3d rpy_align;
      sscanf(data,"[%lf,%lf,%lf]",&rpy_align(0),&rpy_align(1),&rpy_align(2));
      params.R_align = rpy2rot(rpy_align);
    }
    else if((std::string(field))=="rpy_ro")
    {
      Eigen::Vector3d rpy_r0;
      sscanf(data,"[%lf,%lf,%lf]",&rpy_r0(0),&rpy_r0(1),&rpy_r0(2));
      params.R0 = rpy2rot(rpy_r0);
    }
    else if((std::string(field))=="k_north")
    {
      params.K_north = stringToDiag(data);
    }
    else if((std::string(field))=="k_g")
    {
      params.K_g = stringToDiag(data);
    }
    else if ((std::string(field))=="last_mod")
    {

      char str[128];
      sscanf(data,"%s",str);
      params.last_mod = std::string(str);
      params.last_mod.erase(remove(params.last_mod.begin(),params.last_mod.end(), '\"' ),params.last_mod.end());
	
    }
    else if((std::string(field))=="acc_bias")
    {
      Eigen::Vector3d vec;
      sscanf(data,"[%lf,%lf,%lf]",&vec(0),&vec(1),&vec(2));
      params.acc_bias = vec;
    }
    else if((std::string(field))=="ang_bias")
    {
      Eigen::Vector3d vec;
      sscanf(data,"[%lf,%lf,%lf]",&vec(0),&vec(1),&vec(2));
      params.ang_bias = vec;
    }
    else if((std::string(field))=="mag_bias")
    {
      Eigen::Vector3d vec;
      sscanf(data,"[%lf,%lf,%lf]",&vec(0),&vec(1),&vec(2));
      params.mag_bias = vec;
    }
    
  }

  return params;

}


/**
 *
 * @brief Function to print loaded parameters.
 *
 * @param Parameter struct.
 *
 */
void print_loaded_params(config_params params)
{

  printf("***********************************\n");
  printf("           LOADED PARAMS \n");
  printf("***********************************\n");
  printf("  last_mod: %s\n",params.last_mod.c_str());
  printf("        hz: %d (s^-1)\n",params.hz);
  printf("       lat: %+18.12f (deg)   %+18.12f (rad)\n",(180.0/M_PI)*params.lat,params.lat);
  printf("    o_file: %s\n",params.o_file.c_str());
  printf("    i_file: %s\n",params.i_file.c_str());
  printf("        r0: [%+18.12f,%+18.12f,%+18.12f] (rpy)\n",rot2rph(params.R0)(0),rot2rph(params.R0)(1),rot2rph(params.R0)(2));
  printf("   r_align: [%+18.12f,%+18.12f,%+18.12f] (rpy)\n",rot2rph(params.R_align)(0),rot2rph(params.R_align)(1),rot2rph(params.R_align)(2));
  printf("     k_acc: [%+18.12f,%+18.12f,%+18.12f] (diag)\n",params.K_acc(0,0),params.K_acc(1,1),params.K_acc(2,2));
  printf("     k_mag: [%+18.12f,%+18.12f,%+18.12f] (diag)\n",params.K_mag(0,0),params.K_mag(1,1),params.K_mag(2,2));
  printf("k_ang_bias: [%+18.12f,%+18.12f,%+18.12f] (diag)\n",params.K_ang_bias(0,0),params.K_ang_bias(1,1),params.K_ang_bias(2,2));
  printf("k_acc_bias: [%+18.12f,%+18.12f,%+18.12f] (diag)\n",params.K_acc_bias(0,0),params.K_acc_bias(1,1),params.K_acc_bias(2,2));
  printf("k_mag_bias: [%+18.12f,%+18.12f,%+18.12f] (diag)\n",params.K_mag_bias(0,0),params.K_mag_bias(1,1),params.K_mag_bias(2,2));
  printf("     k_E_n: [%+18.12f,%+18.12f,%+18.12f] (diag)\n",params.K_E_n(0,0),params.K_E_n(1,1),params.K_E_n(2,2));
  printf("       k_g: [%+18.12f,%+18.12f,%+18.12f] (diag)\n",params.K_g(0,0),params.K_g(1,1),params.K_g(2,2));
  printf("   k_north: [%+18.12f,%+18.12f,%+18.12f] (diag)\n",params.K_north(0,0),params.K_north(1,1),params.K_north(2,2));
  printf("  acc_bias: [%+18.12f,%+18.12f,%+18.12f] (diag)\n",params.acc_bias(0),params.acc_bias(1),params.acc_bias(2));
  printf("  ang_bias: [%+18.12f,%+18.12f,%+18.12f] (diag)\n",params.ang_bias(0),params.ang_bias(1),params.ang_bias(2));
  printf("  mag_bias: [%+18.12f,%+18.12f,%+18.12f] (diag)\n",params.mag_bias(0),params.mag_bias(1),params.mag_bias(2));

}
