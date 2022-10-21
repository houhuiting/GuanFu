set -o errexit
set -o nounset
set -o pipefail
sudo docker run -it -v $(pwd)/guanfu-rs/mount:/home/rebuild/mount --network host --rm --privileged dettrace:guanfu /bin/bash /home/rebuild/start.sh
#sudo docker run -it --rm ubuntu /bin/bash 