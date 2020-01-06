from pdf2image import convert_from_path
from PIL import Image

images = convert_from_path("../Altium Project/output/prints.pdf")

images[0].thumbnail((300, 300), Image.LANCZOS)
images[1].thumbnail((500, 500), Image.LANCZOS)
images[2].thumbnail((200, 200), Image.LANCZOS)
images[3].thumbnail((200, 200), Image.LANCZOS)

im_out = Image.new('RGB', (20+images[0].width+20+images[2].width+20+images[3].width+20,
                           20+images[1].height+20+images[0].height+20), (255,255,255))

im_out.paste(images[1], (140, 20))
im_out.paste(images[0], (20, 20+images[1].height+20))
im_out.paste(images[2], (20+images[0].width+20,
                         20+images[1].height+20+int(images[0].height/2)-int(images[2].height/2)))
im_out.paste(images[3], (20+images[0].width+20+images[2].width+20,
                         20+images[1].height+20+int(images[0].height/2)-int(images[3].height/2)))
im_out.save("prints_thumbnail_800px.png")
