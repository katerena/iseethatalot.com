#
# AlotMaker is a class for making alots. Duh.
# Construct an alot maker with some configuration variables,
# then reuse the maker over and over on multiple alots.
# The process() method is used to render a specific alot.
# This returns a PIL image, which can be saved to a file
# via result_img.save(FILENAME)
#

# this downloads the images for us
import urllib2
# we use this to detect the image types
import mimetypes
# this is for doing some simple floor/ceiling stuff
import math

# PIL (Python Image Library) for basic image manipulations
from PIL import Image, ImageFont, ImageDraw

import logging

log = logging.getLogger("alot_maker")

# StringIO lets you read strings like a file,
# useful for creating PIL images from downloaded content
# without a temp file
from cStringIO import StringIO


class ProcessError(Exception):
    """just an exception class that we'll be using to raise when
       someone gives us a bad url or file type.
    """
    pass


class AlotMaker(object):
    def __init__(self, overlay, font, tiles_across, logo_text, max_tile_size):
        """overlay is the path to the alot overlay image.
           font is the path to the font to use for the text labels.
           tiles_across is the number of times to tile the images across the alot.
           logo_text is the text to show in the corner of the image.
           max_tile_size is the maximum allowed size (in bytes) of tile images.
        """
        self.tiles_across = tiles_across
        self.logo_text = logo_text
        self.max_tile_size = max_tile_size
        self.supported_types = ('.jpg', '.jpeg', '.jpe', '.png', '.gif', '.bmp')

        # Load some reusable resources
        self.overlay = Image.open(overlay)
        self.label_font = ImageFont.truetype(font, 18)
        self.logo_font = ImageFont.truetype(font, 12)

        log.debug("Alot resources loaded")

    def download_tile(self, url):
        """Downloads the image at the given url.
            Checks that the image is a supported type.
            Doesn't allow downloads over the preset maximum size.
        """
        log.debug("Downloading tile from %s", url)

        #step 1: cut a hole in the box.  Or just download the image.
        try:
            resp = urllib2.urlopen(url)
        except urllib2.URLError:
            #400-500 range error.
            raise ProcessError("Could not fetch %s" % url)

        if resp.getcode() / 100 != 2: #make sure it's a 200-class response.
            raise ProcessError("Could not fetch %s" % url)
            #get the extension from the response type, validate that it's an image

        extension = mimetypes.guess_extension(resp.info().gettype(), strict=False)
        if not extension.lower() in self.supported_types:
            raise ProcessError("%s is of unsupported type %s" %
                               (url, resp.info().gettype()))

        # download ALL the bytes
        contents = resp.read(self.max_tile_size)
        if len(contents) == self.max_tile_size:
            raise ProcessError("%s is too big (more than %d bytes)" % (url, self.max_tile_size))

        # open the image using PIL - need to load it into a file-like stream first
        buffer = StringIO(contents)
        return Image.open(buffer)

    def resize_tile(self, rawImage, targetSize, tilesAcross):
        """Given a loaded image as the basis for the tile,
            the background image's size, and the number of times
            the tile should be arranged horizontally across the image,
            return a resized tile image appropriate for this.
        """
        imageSize = rawImage.size
        tileWidth = int(math.ceil(targetSize[0] / tilesAcross)) # the horizontal width of the tile
        resizeFactor = float(tileWidth) / imageSize[0] # oldSize * resizeFactor = newSize
        tileHeight = int(math.ceil(imageSize[1] * resizeFactor)) # the vertical height of the tile

        log.debug("Resizing image to %s, repeating %d across" % (str((tileWidth, tileHeight)), tilesAcross))

        return rawImage.resize((tileWidth, tileHeight), Image.ANTIALIAS)

    def add_text(self, targetImage, labelText, labelFont, logoText, logoFont):
        """Given an image, draws the label text and logo text on the image,
            in the provided fonts.
        """
        targetSize = targetImage.size

        # add some text
        # these positions are magic
        logoTextMargin = (3, 3)
        labelTextMargin = (8, 25)

        labelTextSize = labelFont.getsize(labelText)
        labelTextPosition = (targetSize[0] - labelTextSize[0] - labelTextMargin[0], labelTextMargin[1])

        logoTextSize = logoFont.getsize(logoText)
        logoTextPosition = (logoTextMargin[0], targetSize[1] - logoTextSize[1] - logoTextMargin[1])

        log.debug("Slapping '%s' and '%s' on the image" % (labelText, logoText))

        targetDraw = ImageDraw.Draw(targetImage)
        targetDraw.text(labelTextPosition, labelText, "black", font=labelFont)
        targetDraw.text(logoTextPosition, logoText, "black", font=logoFont)

    def process(self, tile_image_url, alot_word):
        """Generates an alot given the tile image url and the alot word.
            Returns a PIL image resource.
            Throws exceptions when things go haywire.
        """

        tile = self.download_tile(tile_image_url)

        # resize to tile width, keeping the height proportional
        targetSize = self.overlay.size
        tile = self.resize_tile(tile, targetSize, self.tiles_across)

        # create a blank image of the correct size
        targetImage = Image.new("RGBA", targetSize, "white")

        # infer the number of times the image should be tiled vertically
        tileSize = tile.size
        tilesDown = int(math.ceil(float(targetSize[1]) / tileSize[1]))

        # paste the tile image a bunch of times
        xCovered = 0
        yCovered = 0

        while yCovered < targetSize[1]:
            while xCovered < targetSize[0]:
                targetImage.paste(tile, (xCovered, yCovered))
                xCovered += tileSize[0]
            yCovered += tileSize[1]
            xCovered = 0

        # paste the overlay into the image
        targetImage.paste(self.overlay, (0, 0), self.overlay)

        labelText = ("alot of %s" % (alot_word)).upper()
        self.add_text(targetImage, labelText, self.label_font, self.logo_text, self.logo_font)

        return targetImage


def test():
    # An alot image
    img_url = "https://encrypted-tbn3.gstatic.com/images?q=tbn:ANd9GcRuVVnjpHnbYQH94AgeHL3ADeFJcQOkfMl41gWHI227qvY6y8XkZQ"

    # A too big image
    #img_url = "http://masters.galleries.dpreview.com.s3.amazonaws.com/1545199.jpg?AWSAccessKeyId=14Y3MT0G2J4Y72K3ZXR2&Expires=1375161556&Signature=HyKOFjs0A7ESZDgPhdsm54E3KuQ%3d"

    tiles_across = 5
    overlay = "alot_mask.png"
    font = "source-sans-pro/SourceSansPro-Regular.otf"
    logo_text = "ISEETHATALOT.COM"
    max_tile_size = 10485760 #10 MB

    maker = AlotMaker(overlay, font, tiles_across, logo_text, max_tile_size)
    result = maker.process(img_url, "alots")
    result.save("out.png")


if __name__ == '__main__':
    test()