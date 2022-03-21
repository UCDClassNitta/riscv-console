FROM cjnitta/riscv_base

RUN apt-get update && apt-get install sudo libgtk-3-dev dbus-x11 -y

# Add user so that container does not run as root 
RUN useradd -m docker 

COPY . /code
WORKDIR /code


CMD ["/bin/bash"]
