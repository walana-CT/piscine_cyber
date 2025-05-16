IMAGE_NAME=darkfruit_image
CONTAINER_NAME=darkfruit_container
ONION_PATH=hidden_service/hostname

build:
	sudo docker build -t $(IMAGE_NAME) .

run:
	sudo docker run -d --name $(CONTAINER_NAME) -p 4242:4242 -p 80:80 $(IMAGE_NAME)

stop:
	sudo docker stop $(CONTAINER_NAME) || true
	sudo docker rm $(CONTAINER_NAME) || true

restart: sudo stop build run

logs:
	sudo docker logs -f $(CONTAINER_NAME)

onion:
	@echo "Adresse .onion :"
	@sudo docker exec -it $(CONTAINER_NAME) cat /var/lib/tor/hidden_service/hostname || echo "⚠️ Le service Tor n'est peut-être pas encore prêt."