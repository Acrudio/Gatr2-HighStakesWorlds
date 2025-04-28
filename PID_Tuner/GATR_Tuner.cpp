// Lucas' PID Logging FIles
char* CreatePIDLogFile(float _kP, float _kI, float _kD, float _target, char* _taskName){
    char timeBuffer[100];       // Character array to store the formatted time
    char dtBuffer[100]; 
    time_t rawtime;         // Variable to store raw time (in seconds since epoch)
    struct tm *timeinfo;    // Structure to hold broken-down time
  
    // Get the current time
    time(&rawtime);
  
    // Convert to local time format
    timeinfo = localtime(&rawtime);
  
    // Format the time and store it in buffer
    strftime(dtBuffer, sizeof(dtBuffer), "%m_%d-%H_%M_%S", timeinfo);
  
    sprintf(timeBuffer, "/usd/%s-%s.csv", dtBuffer, _taskName);
    // sprintf(timeBuffer, "%s.csv", dtBuffer);
    char* nameBuffer = static_cast<char *>(malloc(100));
  
    strcpy(nameBuffer, timeBuffer);
  
    FILE *fp;
    fp = fopen(timeBuffer, "w"); // Create the file
  
    sprintf(timeBuffer, "# Kp:%.2f Ki:%.5f Kd:%.2f\n# %.2f\n", _kP, _kI, _kD, _target);
    fputs(timeBuffer, fp);
    fputs("Time,TrackingValue\n", fp);
  
    fclose(fp);
    return nameBuffer;
  }
  char* CreatePIDLogFile(ez::PID::Constants _constants, double _target, char* _taskName){
    return CreatePIDLogFile(_constants.kp, _constants.ki, _constants.kd,_target, _taskName);
  }
  
  void LogPidValue(char *fileName, double _time, double _value){
    FILE *fp;
    fp = fopen(fileName, "a"); // open the file

    
  
    if(fp != NULL){
      char buffer[50];
  
      sprintf(buffer, "%.5f, %.5f\n", _time, _value);
      fputs(buffer, fp);
  
      fclose(fp);
    }
  }

  
  void Pid_LoggingOperation_TASK(ez::PID::Constants constants, char* taskName, function<double(void)> trackingFunction, double target){
    pros::delay(1000);
    
    char *fileName = CreatePIDLogFile(constants,target, taskName);
  
    while (true)
    {
      LogPidValue(fileName, pros::millis(), trackingFunction());
      pros::delay(100);  // This is used for timer calculations!  Keep this ez::util::DELAY_TIME
    }
  
    // free(fileName);
  }