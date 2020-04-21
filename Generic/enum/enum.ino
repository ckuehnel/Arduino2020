// File enum.ino

enum Days{Sun,Mon,tue,Wed,thu,Fri,Sat};
enum Colors{Red=10, Green, Blue};

void setup() 
{
  Serial.begin(115200);
  
  Serial.println(Mon);
  Serial.println(Fri);
  Serial.println(Green);
  
  Colors myFavorite = Red;
  Serial.println(myFavorite);
}

void loop() {}
