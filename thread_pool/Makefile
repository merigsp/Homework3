all: parallel_scheduler.so demo-application

parallel_scheduler.so:
	g++ -std=c++17 -shared -o parallel_scheduler.so parallel_scheduler.cpp

demo-application:
	g++ -std=c++17 -o demo-application demo.cpp parallel_scheduler.so

clean:
	rm -f parallel_scheduler.so demo-application

