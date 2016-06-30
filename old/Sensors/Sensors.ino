void requestBMPVals() {



  setActiveDevice_BMP(1);
  
  if(!bmp.testConnection()){
    bmpValues[(2)+0] = -50.;
    bmpValues[(2)+1] = -50.;
    continue;
  }
  
  bmp.reload();
  bmpValues[(2*idx)+0] = bmp.readTemperature();
  bmpValues[(2*idx)+1] = bmp.readPressure();
  
}


void printBMPInfo(uint8_t dev) {

  Serial.print("BMP Device: ");
  Serial.print(dev);

  Serial.print(",\t T = ");
  Serial.print(bmpValues[(2*dev)+0]);
  Serial.print(" *C,\t");

  Serial.print("p = ");
  Serial.print(bmpValues[(2*dev)+1]);
  Serial.println(" Pa,\t");

}


void printBMPInfo() {

  for(uint8_t idx=0; idx<N_DEVICES_BMP; idx++){
    printBMPInfo(idx);
  }
}


void initDevices_BMP() {

  resetDevices_BMP();
}


void resetDevices_BMP() {

  pinMode( devicePins_BMP[1], OUTPUT ); //
  digitalWrite( devicePins_BMP[1], LOW );
 
}

void setActiveDevice_BMP(uint8_t deviceID) {

  resetDevices_BMP();
  pinMode( devicePins_BMP[deviceID], INPUT ); // using external pull-up to V3.3
  // digitalWrite( devicePins_BMP[deviceID], HIGH ); // V5.0 !!!
  delay(20); //startup time is min 10 ms
}


void resetBMPVals(){

  bmpValues[(2)+0] = -100.;
  bmpValues[(2)+1] = -100.;
}








