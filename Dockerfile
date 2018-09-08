FROM mrpt/mrpt-build-env:minimal-bionic

ENV DEBIAN_FRONTEND noninteractive

RUN apt-get update && \
    apt-get install -y software-properties-common && \
    add-apt-repository -y ppa:joseluisblancoc/mrpt && \
    apt-get update && \
    apt-get install -y libmrpt-dev mrpt-apps
