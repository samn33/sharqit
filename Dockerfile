FROM ubuntu:latest

ARG USER="samn"
ARG UID="1000"
ARG GROUP="samn"
ARG GID="1000"

# create user & group
RUN useradd -m $USER
RUN usermod -u $UID $USER && groupmod -g $GID $GROUP

# home directory
ARG HOME_DIR='/home/samn'
RUN mkdir -p $HOME_DIR
RUN chown -R $UID:$GID $HOME_DIR

# application directory
ARG APP_DIR='/home/samn/sharqit'
RUN mkdir -p $APP_DIR
COPY . $APP_DIR
RUN chown -R $UID:$GID $APP_DIR

ENV HOME $HOME_DIR

RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    git \
    python3-pip \
    nlohmann-json3-dev \
    graphviz \
    libeigen3-dev \
    && rm -rf /var/lib/apt/lists/* \
    && pip install nanobind

# change user
USER $USER

RUN mkdir -p $HOME/bin $HOME/lib $HOME/include/sharqit

WORKDIR $HOME
RUN git clone https://github.com/google/googletest.git \
    && cd googletest \
    && mkdir build \
    && cd build \
    && cmake .. \
    && make \
    && cp -r ../googletest/include/gtest $HOME/include \
    && cp -r lib $HOME

WORKDIR $HOME/sharqit/sharqit/cpp
RUN make && make install

WORKDIR $HOME/sharqit/tests/cpp
ENV LD_LIBRARY_PATH = $LD_LIBRARY_PATH:$HOME/lib
RUN make && ./test_sharqit

WORKDIR $HOME/sharqit
RUN python3 setup.py install --user

WORKDIR $HOME/sharqit/tests/py
RUN ./test.sh

WORKDIR $HOME

CMD ["/bin/bash"]
