#include "wgl_config.h"

#if CUR_GL_TYPE == OPENGL_WGL

BEGIN_GFX_NAMESPACE
BEGIN_GL3_CORE_NAMESPACE

static int findPixelFormatAttribValueWGL(const int* attribs, int attribCount, const int* values, int attrib) {
    int i;

    for (i = 0; i < attribCount; i++) {
        if (attribs[i] == attrib) return values[i];
    }

    CCERROR("WGL: Unknown pixel format attribute requested");

    return 0;
}

#define ADD_ATTRIB(a)                                                       \
    {                                                                       \
        assert((size_t)attribCount < sizeof(attribs) / sizeof(attribs[0])); \
        attribs[attribCount++] = a;                                         \
    }

#define SET_ATTRIB(a, v)      \
    {                         \
        attribs[index++] = a; \
        attribs[index++] = v; \
    }

#define FIND_ATTRIB_VALUE(a) findPixelFormatAttribValueWGL(attribs, attribCount, values, a)

const WGLConfig* wglChooseConfig(const WGLConfig* desired, const WGLConfig* alternatives, unsigned int count) {
    unsigned int i;
    unsigned int missing, leastMissing     = UINT_MAX;
    unsigned int colorDiff, leastColorDiff = UINT_MAX;
    unsigned int extraDiff, leastExtraDiff = UINT_MAX;
    const WGLConfig* current;
    const WGLConfig* closest = NULL;

    for (i = 0; i < count; i++) {
        current = alternatives + i;

        if (desired->stereo > 0 && current->stereo == 0) {
            // Stereo is a hard constraint
            continue;
        }

        // Count number of missing buffers
        {
            missing = 0;

            if (desired->alphaBits > 0 && current->alphaBits == 0) missing++;

            if (desired->depthBits > 0 && current->depthBits == 0) missing++;

            if (desired->stencilBits > 0 && current->stencilBits == 0) missing++;

            if (desired->auxBuffers > 0 && current->auxBuffers < desired->auxBuffers) {
                missing += desired->auxBuffers - current->auxBuffers;
            }

            if (desired->samples > 0 && current->samples == 0) {
                // Technically, several multisampling buffers could be
                // involved, but that's a lower level implementation detail and
                // not important to us here, so we count them as one
                missing++;
            }

            if (desired->transparent != current->transparent) missing++;
        }

        // These polynomials make many small channel size differences matter
        // less than one large channel size difference

        // Calculate color channel size difference value
        {
            colorDiff = 0;

            if (desired->redBits != WGL_DONT_CARE) {
                colorDiff += (desired->redBits - current->redBits) * (desired->redBits - current->redBits);
            }

            if (desired->greenBits != WGL_DONT_CARE) {
                colorDiff += (desired->greenBits - current->greenBits) * (desired->greenBits - current->greenBits);
            }

            if (desired->blueBits != WGL_DONT_CARE) {
                colorDiff += (desired->blueBits - current->blueBits) * (desired->blueBits - current->blueBits);
            }
        }

        // Calculate non-color channel size difference value
        {
            extraDiff = 0;

            if (desired->alphaBits != WGL_DONT_CARE) {
                extraDiff += (desired->alphaBits - current->alphaBits) * (desired->alphaBits - current->alphaBits);
            }

            if (desired->depthBits != WGL_DONT_CARE) {
                extraDiff += (desired->depthBits - current->depthBits) * (desired->depthBits - current->depthBits);
            }

            if (desired->stencilBits != WGL_DONT_CARE) {
                extraDiff += (desired->stencilBits - current->stencilBits) * (desired->stencilBits - current->stencilBits);
            }

            if (desired->accumRedBits != WGL_DONT_CARE) {
                extraDiff += (desired->accumRedBits - current->accumRedBits) * (desired->accumRedBits - current->accumRedBits);
            }

            if (desired->accumGreenBits != WGL_DONT_CARE) {
                extraDiff += (desired->accumGreenBits - current->accumGreenBits) * (desired->accumGreenBits - current->accumGreenBits);
            }

            if (desired->accumBlueBits != WGL_DONT_CARE) {
                extraDiff += (desired->accumBlueBits - current->accumBlueBits) * (desired->accumBlueBits - current->accumBlueBits);
            }

            if (desired->accumAlphaBits != WGL_DONT_CARE) {
                extraDiff += (desired->accumAlphaBits - current->accumAlphaBits) * (desired->accumAlphaBits - current->accumAlphaBits);
            }

            if (desired->samples != WGL_DONT_CARE) {
                extraDiff += (desired->samples - current->samples) * (desired->samples - current->samples);
            }

            if (desired->sRGB && !current->sRGB) extraDiff++;
        }

        // Figure out if the current one is better than the best one found so
        // far Least number of missing buffers is the most important heuristic,
        // then color buffer size match and lastly size match for other buffers

        if (missing < leastMissing)
            closest = current;
        else if (missing == leastMissing) {
            if ((colorDiff < leastColorDiff) || (colorDiff == leastColorDiff && extraDiff < leastExtraDiff)) {
                closest = current;
            }
        }

        if (current == closest) {
            leastMissing   = missing;
            leastColorDiff = colorDiff;
            leastExtraDiff = extraDiff;
        }
    }

    return closest;
}

int choosePixelFormatWGL(HDC hdc, const WGLConfig* fbconfig) {
    const WGLConfig* closest;
    int i, pixelFormat, nativeCount, usableCount = 0, attribCount = 0;
    int attribs[40];
    int values[sizeof(attribs) / sizeof(attribs[0])];

    std::vector<WGLConfig> usableConfigs;
    nativeCount = DescribePixelFormat(hdc, 1, sizeof(PIXELFORMATDESCRIPTOR), NULL);

    if (WGLEW_ARB_pixel_format) {
        ADD_ATTRIB(WGL_SUPPORT_OPENGL_ARB);
        ADD_ATTRIB(WGL_DRAW_TO_WINDOW_ARB);
        ADD_ATTRIB(WGL_PIXEL_TYPE_ARB);
        ADD_ATTRIB(WGL_ACCELERATION_ARB);
        ADD_ATTRIB(WGL_RED_BITS_ARB);
        ADD_ATTRIB(WGL_RED_SHIFT_ARB);
        ADD_ATTRIB(WGL_GREEN_BITS_ARB);
        ADD_ATTRIB(WGL_GREEN_SHIFT_ARB);
        ADD_ATTRIB(WGL_BLUE_BITS_ARB);
        ADD_ATTRIB(WGL_BLUE_SHIFT_ARB);
        ADD_ATTRIB(WGL_ALPHA_BITS_ARB);
        ADD_ATTRIB(WGL_ALPHA_SHIFT_ARB);
        ADD_ATTRIB(WGL_DEPTH_BITS_ARB);
        ADD_ATTRIB(WGL_STENCIL_BITS_ARB);
        ADD_ATTRIB(WGL_ACCUM_BITS_ARB);
        ADD_ATTRIB(WGL_ACCUM_RED_BITS_ARB);
        ADD_ATTRIB(WGL_ACCUM_GREEN_BITS_ARB);
        ADD_ATTRIB(WGL_ACCUM_BLUE_BITS_ARB);
        ADD_ATTRIB(WGL_ACCUM_ALPHA_BITS_ARB);
        ADD_ATTRIB(WGL_AUX_BUFFERS_ARB);
        ADD_ATTRIB(WGL_STEREO_ARB);
        ADD_ATTRIB(WGL_DOUBLE_BUFFER_ARB);

        if (WGLEW_ARB_multisample) ADD_ATTRIB(WGL_SAMPLES_ARB);

        if (WGLEW_ARB_framebuffer_sRGB || WGLEW_EXT_framebuffer_sRGB) ADD_ATTRIB(WGL_FRAMEBUFFER_SRGB_CAPABLE_ARB);
    }
    usableConfigs.resize(nativeCount);

    for (i = 0; i < nativeCount; i++) {
        WGLConfig* u = usableConfigs.data() + usableCount;
        pixelFormat  = i + 1;

        if (WGLEW_ARB_pixel_format) {
            // Get pixel format attributes through "modern" extension

            if (!wglGetPixelFormatAttribivARB(hdc, pixelFormat, 0, attribCount, attribs, values)) {
                CCERROR("WGL: Failed to retrieve pixel format attributes");
                return 0;
            }

            if (!FIND_ATTRIB_VALUE(WGL_SUPPORT_OPENGL_ARB) || !FIND_ATTRIB_VALUE(WGL_DRAW_TO_WINDOW_ARB)) {
                continue;
            }

            if (FIND_ATTRIB_VALUE(WGL_PIXEL_TYPE_ARB) != WGL_TYPE_RGBA_ARB) continue;

            if (FIND_ATTRIB_VALUE(WGL_ACCELERATION_ARB) == WGL_NO_ACCELERATION_ARB) continue;

            if (FIND_ATTRIB_VALUE(WGL_DOUBLE_BUFFER_ARB) != fbconfig->doublebuffer) continue;

            u->redBits   = FIND_ATTRIB_VALUE(WGL_RED_BITS_ARB);
            u->greenBits = FIND_ATTRIB_VALUE(WGL_GREEN_BITS_ARB);
            u->blueBits  = FIND_ATTRIB_VALUE(WGL_BLUE_BITS_ARB);
            u->alphaBits = FIND_ATTRIB_VALUE(WGL_ALPHA_BITS_ARB);

            u->depthBits   = FIND_ATTRIB_VALUE(WGL_DEPTH_BITS_ARB);
            u->stencilBits = FIND_ATTRIB_VALUE(WGL_STENCIL_BITS_ARB);

            u->accumRedBits   = FIND_ATTRIB_VALUE(WGL_ACCUM_RED_BITS_ARB);
            u->accumGreenBits = FIND_ATTRIB_VALUE(WGL_ACCUM_GREEN_BITS_ARB);
            u->accumBlueBits  = FIND_ATTRIB_VALUE(WGL_ACCUM_BLUE_BITS_ARB);
            u->accumAlphaBits = FIND_ATTRIB_VALUE(WGL_ACCUM_ALPHA_BITS_ARB);

            u->auxBuffers = FIND_ATTRIB_VALUE(WGL_AUX_BUFFERS_ARB);

            if (FIND_ATTRIB_VALUE(WGL_STEREO_ARB)) u->stereo = WGL_TRUE;

            if (WGLEW_ARB_multisample) u->samples = FIND_ATTRIB_VALUE(WGL_SAMPLES_ARB);

            if (WGLEW_ARB_framebuffer_sRGB || WGLEW_EXT_framebuffer_sRGB) {
                if (FIND_ATTRIB_VALUE(WGL_FRAMEBUFFER_SRGB_CAPABLE_ARB)) u->sRGB = WGL_TRUE;
            }
        } else {
            // Get pixel format attributes through legacy PFDs

            PIXELFORMATDESCRIPTOR pfd;

            if (!DescribePixelFormat(hdc, pixelFormat, sizeof(PIXELFORMATDESCRIPTOR), &pfd)) {
                CCERROR("WGL: Failed to describe pixel format");
                return 0;
            }

            if (!(pfd.dwFlags & PFD_DRAW_TO_WINDOW) || !(pfd.dwFlags & PFD_SUPPORT_OPENGL)) {
                continue;
            }

            if (!(pfd.dwFlags & PFD_GENERIC_ACCELERATED) && (pfd.dwFlags & PFD_GENERIC_FORMAT)) {
                continue;
            }

            if (pfd.iPixelType != PFD_TYPE_RGBA) continue;

            if (!!(pfd.dwFlags & PFD_DOUBLEBUFFER) != fbconfig->doublebuffer) continue;

            u->redBits   = pfd.cRedBits;
            u->greenBits = pfd.cGreenBits;
            u->blueBits  = pfd.cBlueBits;
            u->alphaBits = pfd.cAlphaBits;

            u->depthBits   = pfd.cDepthBits;
            u->stencilBits = pfd.cStencilBits;

            u->accumRedBits   = pfd.cAccumRedBits;
            u->accumGreenBits = pfd.cAccumGreenBits;
            u->accumBlueBits  = pfd.cAccumBlueBits;
            u->accumAlphaBits = pfd.cAccumAlphaBits;

            u->auxBuffers = pfd.cAuxBuffers;

            if (pfd.dwFlags & PFD_STEREO) u->stereo = WGL_TRUE;
        }

        u->handle = pixelFormat;
        usableCount++;
    }

    if (!usableCount) {
        CCERROR("WGL: The driver does not appear to support OpenGL");

        return 0;
    }

    closest = wglChooseConfig(fbconfig, usableConfigs.data(), usableCount);
    if (!closest) {
        CCERROR("WGL: Failed to find a suitable pixel format");

        return 0;
    }

    pixelFormat = (int)closest->handle;
    return pixelFormat;
}

void setAttribsARB(int* attribs, const WLGCtxconfig& ctxConfig) {
    int index = 0, mask = 0, flags = 0;

    if (ctxConfig.forward) flags |= WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB;

    if (ctxConfig.profile == ProfileType::OPENGL_CORE_PROFILE)
        mask |= WGL_CONTEXT_CORE_PROFILE_BIT_ARB;
    else if (ctxConfig.profile == ProfileType::OPENGL_COMPAT_PROFILE)
        mask |= WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB;

    if (ctxConfig.debug) flags |= WGL_CONTEXT_DEBUG_BIT_ARB;

    if (ctxConfig.robustness != RobustnessType::NO_ROBUSTNESS) {
        if (WGLEW_ARB_create_context_robustness) {
            if (ctxConfig.robustness == RobustnessType::NO_RESET_NOTIFICATION) {
                SET_ATTRIB(WGL_CONTEXT_RESET_NOTIFICATION_STRATEGY_ARB, WGL_NO_RESET_NOTIFICATION_ARB);
            } else if (ctxConfig.robustness == RobustnessType::LOSE_CONTEXT_ON_RESET) {
                SET_ATTRIB(WGL_CONTEXT_RESET_NOTIFICATION_STRATEGY_ARB, WGL_LOSE_CONTEXT_ON_RESET_ARB);
            }

            flags |= WGL_CONTEXT_ROBUST_ACCESS_BIT_ARB;
        }
    }

    if (ctxConfig.release != ReleaseType::ANY_RELEASE_BEHAVIOR) {
        if (WGLEW_ARB_context_flush_control) {
            if (ctxConfig.release == ReleaseType::RELEASE_BEHAVIOR_NONE) {
                SET_ATTRIB(WGL_CONTEXT_RELEASE_BEHAVIOR_ARB, WGL_CONTEXT_RELEASE_BEHAVIOR_NONE_ARB);
            } else if (ctxConfig.release == ReleaseType::RELEASE_BEHAVIOR_FLUSH) {
                SET_ATTRIB(WGL_CONTEXT_RELEASE_BEHAVIOR_ARB, WGL_CONTEXT_RELEASE_BEHAVIOR_FLUSH_ARB);
            }
        }
    }

    if (ctxConfig.major != 1 || ctxConfig.minor != 0) {
        SET_ATTRIB(WGL_CONTEXT_MAJOR_VERSION_ARB, ctxConfig.major);
        SET_ATTRIB(WGL_CONTEXT_MINOR_VERSION_ARB, ctxConfig.minor);
    }
    if (flags) SET_ATTRIB(WGL_CONTEXT_FLAGS_ARB, flags);
    if (mask) SET_ATTRIB(WGL_CONTEXT_PROFILE_MASK_ARB, mask);
    SET_ATTRIB(0, 0);
}
END_GL3_CORE_NAMESPACE
END_GFX_NAMESPACE

#endif  // #if CUR_GL_TYPE == OPENGL_WGL