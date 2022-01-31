FROM ubuntu:18.04
LABEL maintainer="Andreas Seiderer <seiderer@hcm-lab.de>"

# getting required packages, the source code and create a build with default settings
RUN apt-get -y update && apt-get -y upgrade && apt-get install -y mc nano htop build-essential git cmake libcairo-dev libsdl2-dev portaudio19-dev libavcodec-dev libavformat-dev libavdevice-dev libswscale-dev openjdk-8-jdk python3.6-dev && \
    cd ~ && mkdir code && cd code && mkdir SSI && cd SSI && git clone https://github.com/vitastark/mobileSSI.git && \
    mkdir ssi-build && cd ssi-build && cmake ../mobileSSI/ -DOPENSSI_BUILD_EMOVOICE=ON -DOPENSSI_BUILD_LIBLINEAR=ON && mkdir ../mobileSSI/bin_cmake/ && mkdir ../mobileSSI/bin_cmake/Linux && make -j install

RUN cp -r /root/code/SSI/mobileSSI/chains /root/code/SSI/mobileSSI/bin_cmake/Linux/chains && \
    cp -r /root/code/SSI/mobileSSI/data /root/code/SSI/mobileSSI/bin_cmake/Linux/data && \
    cp -r /root/code/SSI/mobileSSI/models /root/code/SSI/mobileSSI/bin_cmake/Linux/models && \
    cp -r /root/code/SSI/mobileSSI/options /root/code/SSI/mobileSSI/bin_cmake/Linux/options && \
    cp -r /root/code/SSI/mobileSSI/1.wav /root/code/SSI/mobileSSI/bin_cmake/Linux/1.wav && \
    cp -r /root/code/SSI/mobileSSI/emovoice.pipeline /root/code/SSI/mobileSSI/bin_cmake/Linux/emovoice.pipeline && \
    cp -r /root/code/SSI/mobileSSI/emovoice.pipeline-config /root/code/SSI/mobileSSI/bin_cmake/Linux/emovoice.pipeline-config && \
    cp -r /root/code/SSI/mobileSSI/emovoice.xml /root/code/SSI/mobileSSI/bin_cmake/Linux/emovoice.xml && \
    cp -r /root/code/SSI/mobileSSI/ssi_last.err /root/code/SSI/mobileSSI/bin_cmake/Linux/ssi_last.err
