#include "./Application/application_config.h"

int main(){	
	AppCarCleaningDisplayInit();
	while(1){
		AppTimerTask();
		AppWhileLoop();
	}
}
