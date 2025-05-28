IMAGE_NAME=cyber-lab
CONTAINER_NAME=cyber-lab-container

.PHONY: build run shell clean

build:
	docker build -t $(IMAGE_NAME) .

run:
	docker run -it --rm --name $(CONTAINER_NAME) --cap-add=NET_ADMIN --network=bridge $(IMAGE_NAME)

shell:
	docker exec -it $(CONTAINER_NAME) /bin/bash

clean:
	docker rm -f $(CONTAINER_NAME) || true
	docker rmi -f $(IMAGE_NAME) || true