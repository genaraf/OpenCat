
#define IMU_SKIP 2

float ypr[3];           // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector

void print6Axis() {
  PT(ypr[0] );
  PT('\t');
  PT(ypr[1] );
  PT('\t');
  PT(ypr[2] );
  //  PT("\t");
  //  PT(aaWorld.x);
  //  PT("\t");
  //  PT(aaWorld.y);
  //  PT("\t");
  //  PTL(aaWorld.z / 1000);
  //  PT('\t');
  //  PT(aaReal.z);
  PTL();
}

bool read_IMU() {
    return false;
}

void readEnvironment() {
  if (checkGyro && !(frame % IMU_SKIP))
    read_IMU();
}

void imuSetup() {
}    
