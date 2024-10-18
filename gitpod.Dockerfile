FROM ubuntu:latest

USER root

RUN apt-get update && \
    apt-get install -y \
    ubuntu-desktop \
    xfce4 \
    xfce4-goodies \
    tightvncserver \
    && apt-get clean

# Set up a VNC server
RUN mkdir -p /root/.vnc && \
    echo "password" | vncpasswd -f > /root/.vnc/passwd && \
    chmod 600 /root/.vnc/passwd

CMD ["bash", "-c", "vncserver :1 -geometry 1280x800 -depth 24 && tail -f /dev/null"]
