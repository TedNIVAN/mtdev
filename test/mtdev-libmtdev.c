#include <linux/input.h>
#include <assert.h>
#include <stdio.h>
#include <libmtdev.h>

static void syn_mt_report(struct mtdev *dev)
{
	struct input_event ev = { {0, 0}, EV_SYN, SYN_MT_REPORT };
	mtdev_put_event(dev, &ev);
}

static void syn(struct mtdev *dev)
{
	struct input_event ev = { {0, 0}, EV_SYN, SYN_REPORT };
	mtdev_put_event(dev, &ev);
}

static void touchpoint(struct mtdev *dev, int x, int y)
{
	struct input_event ev = { {0, 0}, EV_ABS};

	ev.code = ABS_MT_POSITION_X;
	ev.value = x;
	mtdev_put_event(dev, &ev);
	ev.code = ABS_MT_POSITION_Y;
	ev.value = y;
	mtdev_put_event(dev, &ev);
	syn_mt_report(dev);
}

int main(int argc, char **argv)
{
	struct mtdev *dev = mtdev_new();
	struct input_absinfo abs[2] = {
			{ 0, 0, 1000, 0, 0},
			{ 0, 0, 1000, 0, 0}};

	assert(dev);
	mtdev_set_abs(dev, ABS_MT_POSITION_X, abs + 0);
	mtdev_set_abs(dev, ABS_MT_POSITION_Y, abs + 1);

	touchpoint(dev, 10, 10);
	touchpoint(dev, 500, 500);
	syn(dev);

	touchpoint(dev, 20, 20);
	touchpoint(dev, 520, 520);
	syn(dev);

	touchpoint(dev, 20, 20);
	syn(dev);

	touchpoint(dev, 10, 10);
	touchpoint(dev, 500, 500);
	syn(dev);

	touchpoint(dev, 20, 20);
	touchpoint(dev, 520, 520);
	syn(dev);

	assert(!mtdev_empty(dev));
	while (!mtdev_empty(dev)) {
		struct input_event ev;
		mtdev_get_event(dev, &ev);
		printf("%#x %#x %d\n", ev.type, ev.code, ev.value);
	}

	return 0;
}
