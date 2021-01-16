FROM riscv_base

RUN apt-get update && apt-get install sudo libgtk-3-dev dbus-x11 -y

# Add user so that container does not run as root 
RUN useradd -m docker 
RUN echo "docker:test" | chpasswd 
RUN usermod -s /bin/bash docker 
RUN usermod -aG sudo docker 
ENV HOME /home/docker


COPY . /home/docker
WORKDIR /home/docker

# change to the docker user
USER docker

CMD ["/bin/bash"]
