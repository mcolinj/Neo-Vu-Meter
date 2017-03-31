def linear_scale_to_range(low_cutoff, high_cutoff, num_pixels, sensor_value):
    """
    This function scales a sensor value between the low_cutoff
    and high_cutoff in order to determine which pixels to light
    in a linear scaled VuMeter.
    
    Must display 1 pixel (index 0) at the low cutoff or lower and
    num_pixels-1 at the high_cutoff or higher.   Linear scale between them.
    """
    values_in_range = high_cutoff - low_cutoff + 1
    ticks_per_pixel = values_in_range / (num_pixels - 1)  # 0 to (num_pixels-1)

    #
    # Shift sensor value down to cutoff (base noise level from calibration?)
    # Consider the value to be 0 for anything lower than the cuttoff.
    #
    if sensor_value >= low_cutoff:
        sensor_ticks = sensor_value - low_cutoff
    else:
        sensor_ticks = 0

    #
    # Compute index of the last pixel to display
    # Make sure to set the ceiling at the last pixel.
    #
    pixels = sensor_ticks / ticks_per_pixel
    if pixels > (num_pixels - 1):
        pixels = num_pixels - 1
        
    return pixels

def test_linear_scaling():
    """
    Assuming low cuttoff of 100, and high cuttoff of 500, scale
    the sensor value i so that it maps to a pixel index to display
    uniformly over 20 pixels.    Try out all values between 90 and 510
    and display the results.
    """
    for i in range(90, 510):
        print i, "=>", linear_scale_to_range(100, 500, 20, i)
