FROM riscv_base

RUN apt-get update && apt-get install libgtk-3-dev dbus-x11 -y

COPY . /code/
WORKDIR /code

CMD ["/bin/bash"]
