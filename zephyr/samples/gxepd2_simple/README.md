# GxEPD2 Simple Example - ALL 37 Displays Available!

The **easiest** way to use e-paper displays on Zephyr. **No device tree editing!**

## What's Included

✨ **ALL 37 original display drivers** from `src/epd/` ready to use!  
✨ Just uncomment **ONE line** in `prj.conf`  
✨ Auto-configured pins for Nordic boards  
✨ Works out of the box!

## Quick Start (30 Seconds!)

### Step 1: Choose Your Display

Edit `prj.conf` and uncomment YOUR display:

```ini
# Uncomment ONE:
CONFIG_GXEPD2_154=y                 # 1.54" 200x200 ← Currently selected
# CONFIG_GXEPD2_213_T5D=y           # 2.13" 250x122
# CONFIG_GXEPD2_290_T5=y            # 2.9" 296x128
# CONFIG_GXEPD2_420=y               # 4.2" 400x300
# ... 33 more available!
```

### Step 2: Build

```bash
west build -b nrf52840dk_nrf52840
```

### Step 3: Flash

```bash
west flash
```

**Done!** That's literally it! 🎉

See full README in file for all 37 available displays and details.
