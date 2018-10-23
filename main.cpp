

/*
*
* YUYV画图代码
*
*
*
*/
INT graphic_draw_line_YUYV
(UINT8 *img_buf,
UINT   img_width,
UINT   img_height,
UINT   img_pitch,
INT    x1,
INT    y1,
INT    x2,
INT    y2,
UINT   thickness,
UINT8 color_r,
UINT8 color_g,
UINT8 color_b
)
{
	UINT8 *pixel;
	INT w, h, dx, dy;
	INT x, y;
	INT i, a, b;
	INT f;	/* fraction part */
	INT width = (INT)img_width;
	INT height = (INT)img_height;
	INT pitch = (INT)img_pitch;
	INT th2 = (INT)(thickness / 2);
	UINT16 c0 = color_r;
	UINT16 c1 = color_g;
	UINT16 c2 = color_b;
	UINT32 size = 0;
	if (!img_buf) {
		/* invalid parameters */
		return -1;
	}

	if ((x1 < 0) || (x2 < 0) || (y1 < 0) || (y2 < 0)
		|| (x1 >= width) || (x2 >= width)
		|| (y1 >= height) || (y2 >= height)) {
		/* out of boundary */
		return -1;
	}

	if (!thickness) {
		/* thickness == 0. It's NOT a line. */
		return 0;
	}

	(x2 >= x1) ? (w = x2 - x1, dx = 1) : (w = x1 - x2, dx = -1);
	(y2 >= y1) ? (h = y2 - y1, dy = 1) : (h = y1 - y2, dy = -1);

	x = x1;
	y = y1;
	if (!w && !h) {
		/* this means (x1 == x2) && (y1 == y2). It's NOT a line. */
		return 0;
	}
	else if (!w) {
		/* vertical line */
		a = x - th2;
		b = a + thickness;
		a = (a >= 0) ? a : 0;
		b = (b < width) ? b : (width - 1);
		while (y != y2) {
			/* get address of pixel(a, y) */
			size = (y * pitch) + (a * 2);
			pixel = (img_buf + (y * pitch) + (a * 2));

			/* draw pixels */
			for (i = a; i <= b; i++) {
				pixel[0] = c0;
				if (size % 4 == 0)
				{
					pixel[1] = c1;
					pixel[3] = c2;
				}
				else
				{
					*(pixel-1) = c1;
					*(pixel+1) = c2;
				}
				size += 2;
				pixel+=2;
			}
			y += dy;
		}
	}
	else if (!h) {
		/* horizontal line */
		a = y - th2;
		b = a + thickness;
		a = (a >= 0) ? a : 0;
		b = (b < height) ? b : (height - 1);
		while (x != x2) {
			/* get address of pixel(x, a) */

			//pixel = (UINT16 *)(img_buf + (y * pitch * 2) + (x * 2));
			pixel = (img_buf + (a * pitch) + x*2);
			size = (a * pitch) + x * 2;
			/* draw pixels */
			for (i = a; i <= b; i++) {
				pixel[0] = c0;
				if (size % 4 == 0)
				{
					pixel[1] = c1;
					pixel[3] = c2;
				}
				else
				{
					*(pixel - 1) = c1;
					*(pixel + 1) = c2;
				}
				size += pitch;
				pixel = ((UINT8 *)pixel + pitch);
			}
			x += dx;
		}
	}
	else if (w == h) {
		while (x != x2) {
			a = x - th2;
			b = a + thickness;
			a = (a >= 0) ? a : 0;
			b = (b < width) ? b : (width - 1);
			/* get address of pixel(a, y) */
			pixel = (img_buf + (y * pitch) + (a * 2));
			size = (y * pitch) + (a * 2);
			//			pixel = img_buf + (y * pitch)%4 + a;
			/* draw pixels */
			for (i = a; i <= b; i++) {
				pixel[0] = c0;
				if (size % 4 == 0)
				{
					pixel[1] = c1;
					pixel[3] = c2;
				}
				else
				{
					*(pixel - 1) = c1;
					*(pixel + 1) = c2;
				}
				size += 2;
				pixel+=2;
			}
			x += dx;
			y += dy;
		}
	}
	else if (w > h) {
		f = -(w >> 1);
		while (x != x2) {
			a = y - th2;
			b = a + thickness;
			a = (a >= 0) ? a : 0;
			b = (b < height) ? b : (height - 1);
			/* get address of pixel(x, a) */
			pixel = img_buf + (a * pitch) + x*2;
			size = (a * pitch) + x * 2;
			//pixel = (UINT16 *)(img_buf + (y * pitch) + (x * 2));
			/* draw pixels */
			for (i = a; i <= b; i++) {
				pixel[0] = c0;
				if (size % 4 == 0)
				{
					pixel[1] = c1;
					pixel[3] = c2;
				}
				else
				{
					*(pixel - 1) = c1;
					*(pixel + 1) = c2;
				}
				size += pitch;
				pixel = ((UINT8 *)pixel + pitch);
			}
			x += dx;
			f += h;
			if (f >= 0) {
				f -= w;
				y += dy;
			}
		}
	}
	else { /* w < h */
		f = -(h >> 1);
		while (y != y2) {
			a = x - th2;
			b = a + thickness;
			a = (a >= 0) ? a : 0;
			b = (b < width) ? b : (width - 1);
			/* get address of pixel(a, y) */

			pixel = (img_buf + (y * pitch) + (a * 2));
			size = (y * pitch) + (a * 2);
			/* draw pixels */
			for (i = a; i <= b; i++) {
				pixel[0] = c0;
				if (size % 4 == 0)
				{
					pixel[1] = c1;
					pixel[3] = c2;
				}
				else
				{
					*(pixel - 1) = c1;
					*(pixel + 1) = c2;
				}
				size += 2;
				pixel+=2;
			}
			y += dy;
			f += w;
			if (f >= 0) {
				f -= h;
				x += dx;
			}
		}
	}

	return 0;
}


int main()
{
  //打开YUYV图像
	FILE *fp = fopen("img\\c1h0","r");

	unsigned char *p = (unsigned char *)malloc(1280*960*2);
	fread(p, 1280 * 960 * 2, 1, fp);
	fclose(fp);

	graphic_draw_line_YUYV(p, 1280, 960, 1280 * 2, 0, 0, 1279, 0, 1, 0,0,255);
	graphic_draw_line_YUYV(p, 1280, 960, 1280 * 2, 0, 0, 0, 959, 5, 0, 0, 255);
	graphic_draw_line_YUYV(p, 1280, 960, 1280 * 2, 0, 959, 1279, 959,1, 255, 0, 0);
	graphic_draw_line_YUYV(p, 1280, 960, 1280 * 2, 1279, 0, 1279, 959, 1, 0, 0, 255);


	graphic_draw_line_YUYV(p, 1280, 960, 1280 * 2, 300,471, 1279, 300, 1, 0, 0, 255);
	graphic_draw_line_YUYV(p, 1280, 960, 1280 * 2, 165,232, 500, 959, 5, 0, 0, 255);
	graphic_draw_line_YUYV(p, 1280, 960, 1280 * 2, 280, 432, 600, 546, 1, 255, 0, 0);
	graphic_draw_line_YUYV(p, 1280, 960, 1280 * 2, 354, 451, 741, 959, 1, 0, 0, 255);

	//cyf_drawRect_to_YUVsp422(p, 1280, 960, rect, color);

	FILE *fp1 = fopen("final", "w");
	fwrite(p, 1280 * 960 * 2, 1, fp1);
	fclose(fp1);

	return 0;
}
