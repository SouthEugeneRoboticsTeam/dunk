FROM mrpt/mrpt-build-env:minimal-bionic

RUN apt-get install software-properties-common -y && \
    add-apt-repository ppa:joseluisblancoc/mrpt -y && \
    apt-get update && \
    apt-get install libmrpt-dev mrpt-apps -y
