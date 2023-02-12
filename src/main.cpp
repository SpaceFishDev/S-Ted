#include<Renderer.h>
int main(){
	Renderer r = Renderer(1000,650, "VeraMono.ttf");
	while(!r.Quit){
		r.Update();
	}
}
