import numpy as np
from scipy import misc


def get_hazy_free(hazy, ml, atmos):
    row, col = ml.shape
    hazy_free = np.array(hazy)

    for i in range(row):
        for j in range(col):
            for c in range(3):
                hazy_free[i, j, c] = (hazy[i, j, c] - ml[i, j]) / (1 - ml[i, j] / atmos)

    return hazy_free


def get_atmos(mave, mmax, epsilon):

    return epsilon * mmax.max() + (1 - epsilon) * mave.max()


def get_ml(mmin, mave, mav):
    row, col = mmin.shape
    ml = np.zeros((row, col))
    rho = 1.2  # random.uniform(0, 1 / mav)
    min_rm = min(rho * mav, 0.9)

    for i in range(row):
        for j in range(col):
            ml[i, j] = min(min_rm * mave[i, j], mmin[i, j])

    return ml


def get_mm(hazy):
    row, col = hazy.shape[0], hazy.shape[1]
    mmin = np.zeros((row, col))
    mmax = np.zeros((row, col))

    for i in range(row):
        for j in range(col):
            mmin[i, j] = hazy[i, j, 0:3].min()
            mmax[i, j] = hazy[i, j, 0:3].max()

    return mmin, mmax


def get_mave(mmin, k):
    row, col = mmin.shape
    mave = np.zeros(mmin.shape)

    for i in range(row):
        for j in range(col):
            rl = (i - k) if (i - k) >= 0 else 0
            ru = (i + k + 1) if (i + k + 1) <= row else row
            cl = (j - k) if (j - k) >= 0 else 0
            cu = (j + k + 1) if (j + k + 1) <= col else col

            # n = (ru - rl + 1) * (cu - cl + 1)
            mave[i, j] = mmin[rl:ru, cl:cu].mean()

    return mave


def get_dark_channel(hazy, size_patch):
    row, col, tmp = hazy.shape
    dark_channel = np.array(hazy)

    for i in range(row / size_patch):
        for j in range(col / size_patch):
            dark_channel[size_patch*i:size_patch*(i+1), size_patch*j:size_patch*(j+1), 0:3]\
                = hazy[size_patch*i:size_patch*(i+1), size_patch*j:size_patch*(j+1), 0:3].min()

        if col > size_patch * (j+1):
            dark_channel[size_patch*i:size_patch*(i+1), size_patch*(j+1):col, 0:3]\
                = hazy[size_patch*i:size_patch*(i+1), size_patch*(j+1):col, 0:3].min()

    if row > size_patch * (i+1):
        for j in range(col / size_patch):
            dark_channel[size_patch*(i+1):row, size_patch*j:size_patch*(j+1), 0:3]\
                = hazy[size_patch*(i+1):row, size_patch*j:size_patch*(j+1), 0:3].min()
        if col > size_patch * (j+1):
            dark_channel[size_patch*(i+1):row, size_patch*(j+1):col, 0:3]\
                = hazy[size_patch*(i+1):row, size_patch*(j+1):col, 0:3].min()

    return dark_channel


def main():
    id_imgs = range(7, 8)
    k = 5

    for id_img in id_imgs:
        print(id_img)
        hazy = misc.imread('../images/hazy/%d.png' % id_img)
        # print(hazy[0:10, 0:10, :])
        matrix_min, matrix_max = get_mm(hazy)
        # print(matrix_min[0:10, 0:10])
        matrix_ave = get_mave(matrix_min, k)
        # print(matrix_ave[0:10, 0:10])
        mav = matrix_min.mean() / 255

        ml = get_ml(matrix_min, matrix_ave, mav)
        atmos = get_atmos(matrix_min, matrix_ave, 0.05)
        # print('atmos: ' + str(atmos))
        hazy_free = get_hazy_free(hazy, ml, atmos)

        misc.imsave('../images/dark_channel/%d.png' % id_img, matrix_ave, 'png')
        misc.imsave('../images/hazy_free/%d.png' % id_img, hazy_free, 'png')

main()
