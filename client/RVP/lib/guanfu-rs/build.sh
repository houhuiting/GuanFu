set -o errexit
set -o nounset
set -o pipefail
runassudo() {
	sudo docker build -t dettrace:guanfu $(pwd)/guanfu-rs/
	sudo docker run -it -v $(pwd)/guanfu-rs/mount:/home/rebuild/mount --network host --rm --privileged dettrace:guanfu /bin/bash /home/rebuild/start.sh
}
run() {
	docker build -t dettrace:guanfu $(pwd)/guanfu-rs/
	docker run -it -v $(pwd)/guanfu-rs/mount:/home/rebuild/mount --network host --rm --privileged dettrace:guanfu /bin/bash /home/rebuild/start.sh
}
runassu() {
	su docker build -t dettrace:guanfu $(pwd)/guanfu-rs/
	su docker run -it -v $(pwd)/guanfu-rs/mount:/home/rebuild/mount --network host --rm --privileged dettrace:guanfu /bin/bash /home/rebuild/start.sh
}
((EUID == 0)) && run
if type -P sudo >/dev/null; then
	runassudo
else
	runassu
fi