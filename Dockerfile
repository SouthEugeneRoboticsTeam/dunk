FROM mrpt/mrpt-build-env:minimal-bionic

RUN apt-get update && \
    apt-get install -y software-properties-common \
      python-software-properties && \
    add-apt-repository -y ppa:joseluisblancoc/mrpt && \
    apt-get update && \
    apt-get install -y libmrpt-dev mrpt-apps
