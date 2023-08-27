FROM debian:stable-slim
ARG TARGET=orangepi
ARG BOARD_ID=0
ARG GPIO_PIN=3

WORKDIR /app
COPY . /app

RUN rm -r .git && \
    apt-get update && \
    apt-get install git -y && \
    apt-get install sudo -y && \
    apt-get install build-essential -y && \
    echo "#define" "PIN" "$GPIO_PIN" >> Core/config.h &&\
    if [ "$TARGET" = "orangepi" ]; \
	then \
		git clone https://github.com/orangepi-xunlong/wiringOP.git && \
		cd wiringOP && \
		(echo $BOARD_ID | ./build) && \
		cd .. && \
		rm -r wiringOP; \
	fi && \
	if [ "$TARGET" = "raspberrypi" ]; \
	then \
		git clone https://github.com/WiringPi/WiringPi.git && \
		cd WiringPi && \
		./build && \
		cd .. && \
		rm -r WiringPi; \
	fi && \
	make && \
	sudo apt-get remove build-essential git -y && sudo apt autoremove -y && sudo apt autoclean -y && \
	rm -rf /var/cache/apt
CMD ["./Bin/smartfan"]
