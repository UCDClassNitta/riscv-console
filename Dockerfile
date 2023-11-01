FROM cjnitta/riscv_base

# Install packages and add user so that container does not run as root 
RUN apt-get update && apt-get install sudo libgtk-3-dev dbus-x11 -y --no-install-recommends && \
    apt-get clean && \
    rm -rf /var/lib/apt/lists/\* /tmp/\* /var/tmp/* && \
    useradd -m docker 

COPY . /code
WORKDIR /code


CMD ["/bin/bash"]
