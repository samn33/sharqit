import matplotlib.pyplot as plt

ORI_LABEL="original"
PYZX_LABEL="pyzx-0.8.0"
SHARQIT_ZX_LABEL="sharqit-0.2.1 (zx)"
SHARQIT_PP_LABEL="sharqit-0.2.1 (pp)"

def main():

    with open("result_pyzx.csv", mode='r') as f:
        lines_pyzx = [s.rstrip() for s in f.readlines()]

    with open("result_sharqit_zx.csv", mode='r') as f:
        lines_sharqit_zx = [s.rstrip() for s in f.readlines()]

    with open("result_sharqit_pp.csv", mode='r') as f:
        lines_sharqit_pp = [s.rstrip() for s in f.readlines()]

    names = []
    time_ori = []
    time_pyzx = []
    time_sharqit_zx = []
    time_sharqit_pp = []

    tcount_ori = []
    tcount_pyzx = []
    tcount_sharqit_zx = []
    tcount_sharqit_pp = []

    twoqcount_ori = []
    twoqcount_pyzx = []
    twoqcount_sharqit_zx = []
    twoqcount_sharqit_pp = []

    gcount_ori = []
    gcount_pyzx = []
    gcount_sharqit_zx = []
    gcount_sharqit_pp = []

    x0 = []
    x1 = []
    x2 = []
    x3 = []
    
    for i, (line_pyzx, line_sharqit_zx, line_sharqit_pp) in enumerate(zip(lines_pyzx[1:], lines_sharqit_zx[1:], lines_sharqit_pp[1:])):
        token_pyzx = line_pyzx.split(',')
        token_sharqit_zx = line_sharqit_zx.split(',')
        token_sharqit_pp = line_sharqit_pp.split(',')

        x0.append(i - 0.3)
        x1.append(i - 0.1)
        x2.append(i + 0.1)
        x3.append(i + 0.3)

        names.append(token_pyzx[0])

        time_ori.append(0.0)
        tcount_ori.append(int(token_pyzx[2]))
        twoqcount_ori.append(int(token_pyzx[4]))
        gcount_ori.append(int(token_pyzx[6]))

        time_pyzx.append(float(token_pyzx[1]))
        tcount_pyzx.append(int(token_pyzx[3]))
        twoqcount_pyzx.append(int(token_pyzx[5]))
        gcount_pyzx.append(int(token_pyzx[7]))

        time_sharqit_zx.append(float(token_sharqit_zx[1]))
        tcount_sharqit_zx.append(int(token_sharqit_zx[3]))
        twoqcount_sharqit_zx.append(int(token_sharqit_zx[5]))
        gcount_sharqit_zx.append(int(token_sharqit_zx[7]))

        time_sharqit_pp.append(float(token_sharqit_pp[1]))
        tcount_sharqit_pp.append(int(token_sharqit_pp[3]))
        twoqcount_sharqit_pp.append(int(token_sharqit_pp[5]))
        gcount_sharqit_pp.append(int(token_sharqit_pp[7]))

    fig = plt.figure(figsize=[10,12])
    fig.subplots_adjust(top=0.95, bottom=0.185, hspace=0.65, wspace=0.3)

    # Optimization Time plot
    
    ax1 = fig.add_subplot(2, 2, 1)
    ax1.set_ylabel("Time (sec)", fontsize=14)

    g0 = ax1.bar(x0, time_ori, label=ORI_LABEL, align='edge', width=0.2)
    g1 = ax1.bar(x1, time_pyzx, label=PYZX_LABEL, align='edge', width=0.2)
    g2 = ax1.bar(x2, time_sharqit_zx, label=SHARQIT_ZX_LABEL, align='edge', width=0.2)
    g3 = ax1.bar(x3, time_sharqit_pp, label=SHARQIT_PP_LABEL, align='edge', width=0.2)

    ax1.set_xticks(range(len(names)))
    ax1.set_xticklabels(names, rotation=90)
    ax1.set_yscale('log')
    ax1.legend(handles=[g1,g2,g3], loc='upper left', fontsize=10)
    ax1.grid(axis='y')
    
    # T-Count plot

    ax2 = fig.add_subplot(2, 2, 2)
    ax2.set_ylabel("T-Count", fontsize=14)

    g0 = ax2.bar(x0, tcount_ori, label=ORI_LABEL, align='edge', width=0.2)
    g1 = ax2.bar(x1, tcount_pyzx, label=PYZX_LABEL, align='edge', width=0.2)
    g2 = ax2.bar(x2, tcount_sharqit_zx, label=SHARQIT_ZX_LABEL, align='edge', width=0.2)
    g3 = ax2.bar(x3, tcount_sharqit_pp, label=SHARQIT_PP_LABEL, align='edge', width=0.2)

    ax2.set_ylim(0, 1300)
    ax2.set_xticks(range(len(names)))
    ax2.set_xticklabels(names, rotation=90)
    ax2.legend(handles=[g0,g1,g2,g3], loc='upper left', fontsize=10)
    ax2.grid(axis='y')

    # 2Q-Count plot
    
    ax3 = fig.add_subplot(2, 2, 3)
    ax3.set_ylabel("2Q-Count", fontsize=14)

    g0 = ax3.bar(x0, twoqcount_ori, label=ORI_LABEL, align='edge', width=0.2)
    g1 = ax3.bar(x1, twoqcount_pyzx, label=PYZX_LABEL, align='edge', width=0.2)
    g2 = ax3.bar(x2, twoqcount_sharqit_zx, label=SHARQIT_ZX_LABEL, align='edge', width=0.2)
    g3 = ax3.bar(x3, twoqcount_sharqit_pp, label=SHARQIT_PP_LABEL, align='edge', width=0.2)

    ax3.set_ylim(0, 2500)
    ax3.set_xticks(range(len(names)))
    ax3.set_xticklabels(names, rotation=90)
    ax3.legend(handles=[g0,g1,g2,g3], loc='upper left', fontsize=10)
    ax3.grid(axis='y')
    
    # G-Count plot
    
    ax4 = fig.add_subplot(2, 2, 4)
    ax4.set_ylabel("Gate-Count", fontsize=14)
    g0 = ax4.bar(x0, gcount_ori, label=ORI_LABEL, align='edge', width=0.2)
    g1 = ax4.bar(x1, gcount_pyzx, label=PYZX_LABEL, align='edge', width=0.2)
    g2 = ax4.bar(x2, gcount_sharqit_zx, label=SHARQIT_ZX_LABEL, align='edge', width=0.2)
    g3 = ax4.bar(x3, gcount_sharqit_pp, label=SHARQIT_PP_LABEL, align='edge', width=0.2)

    ax4.set_ylim(0, 3500)
    ax4.set_xticks(range(len(names)))
    ax4.set_xticklabels(names, rotation=90)
    ax4.legend(handles=[g0,g1,g2,g3], loc='upper left', fontsize=10)
    ax4.grid(axis='y')

    # show and save
    
    #fig.tight_layout()
    plt.show()
    fig.savefig("plot.png")
    
if __name__ == '__main__':
    main()

