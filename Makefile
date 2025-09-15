controllers/four_wheels_collision_avoidance/JavinoCLibrary/lib/libjavino.a:
	cd controllers/four_wheels_collision_avoidance/ &&\
	git clone https://github.com/chon-group/JavinoInC &&\
	cd JavinoInC &&\
	make all

dependencies: controllers/four_wheels_collision_avoidance/JavinoCLibrary/lib/libjavino.a


