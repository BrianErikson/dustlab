FROM debian:testing
MAINTAINER briankerikson@gmail.com

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get upgrade -y && apt-get install -y binutils cmake make gcc g++ bash \
    libsdl2-dev libglew-dev openssh-server gdb gdbserver rsync git libglm-dev libsoil-dev \
    libopencv-dev libnoise-dev && rm -rf /var/lib/apt/lists* && rm -rf /var/cache/debconf/*-old

# OpenGL support
# replace with other Ubuntu version if desired
# see: https://hub.docker.com/r/nvidia/opengl/
# e.g. nvidia/opengl:1.1-glvnd-runtime-ubuntu16.04)
COPY --from=machinekoder/nvidia-opengl-docker:1.1-glvnd-runtime-stretch \
  /usr/local/lib/x86_64-linux-gnu \
  /usr/local/lib/x86_64-linux-gnu

# replace with other Ubuntu version if desired
# see: https://hub.docker.com/r/nvidia/opengl/
# e.g. nvidia/opengl:1.1-glvnd-runtime-ubuntu16.04
COPY --from=machinekoder/nvidia-opengl-docker:1.1-glvnd-runtime-stretch \
  /usr/local/share/glvnd/egl_vendor.d/10_nvidia.json \
  /usr/local/share/glvnd/egl_vendor.d/10_nvidia.json

# nvidia-container-runtime
ENV NVIDIA_VISIBLE_DEVICES all
ENV NVIDIA_DRIVER_CAPABILITIES compute,utility,graphics
ENV NVIDIA_REQUIRE_CUDA "cuda>=10.2 brand=tesla,driver>=384,driver<385 brand=tesla,driver>=396,driver<397 brand=tesla,driver>=410,driver<411 brand=tesla,driver>=418,driver<419"
ENV PATH /usr/local/nvidia/bin:/usr/local/cuda/bin:${PATH}
ENV LD_LIBRARY_PATH /usr/local/nvidia/lib:/usr/local/nvidia/lib64:${LD_LIBRARY_PATH}

RUN ldconfig && echo "X11UseLocalhost yes" >> /etc/ssh/sshd_config \
    && echo "PermitRootLogin yes" >> /etc/ssh/sshd_config && mkdir /var/run/sshd \
    && echo "root:root" | chpasswd && ssh-keygen -A && \
    echo "/usr/local/nvidia/lib" >> /etc/ld.so.conf.d/nvidia.conf && \
    echo "/usr/local/nvidia/lib64" >> /etc/ld.so.conf.d/nvidia.conf \
    echo NVIDIA_VISIBLE_DEVICES="${NVIDIA_VISIBLE_DEVICES}" >> /etc/environment && \
    echo NVIDIA_DRIVER_CAPABILITIES="${NVIDIA_DRIVER_CAPABILITIES}" >> /etc/environment && \
    echo NVIDIA_REQUIRE_CUDA="${NVIDIA_REQUIRE_CUDA}" >> /etc/environment && \
    echo PATH="${PATH}:\$PATH" >> /etc/profile && \
    echo LD_LIBRARY_PATH="${LD_LIBRARY_PATH}:\$LD_LIBRARY_PATH" >> /etc/profile && \
    echo '/usr/local/lib/x86_64-linux-gnu' >> /etc/ld.so.conf.d/glvnd.conf && \
    ldconfig && \
    echo '/usr/local/$LIB/libGL.so.1' >> /etc/ld.so.preload && \
    echo '/usr/local/$LIB/libEGL.so.1' >> /etc/ld.so.preload

RUN git clone https://github.com/skypjack/entt.git --branch v3.5.2 --depth 1 \
    && mkdir -p entt/build && cd entt/build && cmake -DCMAKE_INSTALL_PREFIX=/usr ../ \
    && make -j$(nproc) install && cd ../../ && rm -r entt

RUN ln -s /usr/include/libnoise /usr/include/noise && ln -s /usr/include/opencv4 /usr/include/opencv4/opencv

RUN git clone https://github.com/CaseyCarter/cmcstl2.git --depth 1 \
    && mkdir -p cmcstl2/build && cd cmcstl2/build && cmake -DCMAKE_INSTALL_PREFIX=/usr ../ \
    && make -j $(nproc) install && cd ../../ && rm -r cmcstl2

EXPOSE 22
ENTRYPOINT exec /usr/sbin/sshd -D -e "$@"
