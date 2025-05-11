import argparse
import os
import re
from urllib.parse import urljoin, urlparse
from urllib.request import urlopen, urlretrieve
from html.parser import HTMLParser

# Liste des extensions autorisées
VALID_EXTENSIONS = ['.jpg', '.jpeg', '.png', '.gif', '.bmp']
VISITED_URLS = set()

class ImageSpider(HTMLParser):
    def __init__(self, base_url):
        super().__init__()
        self.base_url = base_url
        self.images = []
        self.links = []

    def handle_starttag(self, tag, attrs):
        attrs = dict(attrs)
        if tag == 'img' and 'src' in attrs:
            img_url = urljoin(self.base_url, attrs['src'])
            if any(img_url.lower().endswith(ext) for ext in VALID_EXTENSIONS):
                self.images.append(img_url)
        elif tag == 'a' and 'href' in attrs:
            href = urljoin(self.base_url, attrs['href'])
            self.links.append(href)

def download_image(url, path):
    try:
        filename = os.path.basename(urlparse(url).path)
        filepath = os.path.join(path, filename)
        if not os.path.exists(filepath):
            print(f"📥 downloading {url} → {filepath}")
            urlretrieve(url, filepath)
    except Exception as e:
        print(f"⚠️ Dowload error for {url}: {e}")

def crawl(url, path, depth):
    if depth < 0 or url in VISITED_URLS:
        return
    VISITED_URLS.add(url)
    try:
        response = urlopen(url)
        content_type = response.headers.get("Content-Type", "")
        if "text/html" not in content_type:
            return
        html = response.read().decode("utf-8", errors="ignore")
        parser = ImageSpider(url)
        parser.feed(html)
        for img_url in parser.images:
            download_image(img_url, path)
        for link in parser.links:
            if urlparse(link).netloc == urlparse(url).netloc:
                crawl(link, path, depth - 1)
    except Exception as e:
        print(f"⚠️ Erreur lors de l'accès à {url}: {e}")

def main():
    parser = argparse.ArgumentParser(description="Téléchargeur d'images récursif")
    parser.add_argument("url", help="Starting URL")
    parser.add_argument("-r", action="store_true", help="Enable recursive mode")
    parser.add_argument("-l", type=int, default=5, help="Maximal depth default 5")
    parser.add_argument("-p", type=str, default="./data", help="Saving path (default ./data)")
    args = parser.parse_args()

    save_path = args.p
    os.makedirs(save_path, exist_ok=True)

    if args.r:
        crawl(args.url, save_path, args.l)
    else:
        # Mode non récursif
        try:
            response = urlopen(args.url)
            html = response.read().decode("utf-8", errors="ignore")
            parser = ImageSpider(args.url)
            parser.feed(html)
            for img_url in parser.images:
                download_image(img_url, save_path)
        except Exception as e:
            print(f"⚠️ Erreur lors de l'accès à {args.url}: {e}")

if __name__ == "__main__":
    main()


