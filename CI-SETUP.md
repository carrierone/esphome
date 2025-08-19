# CI Setup for Personal Fork

This document explains the CI modifications made to avoid GitHub paid features while maintaining functionality.

## Changes Made for Free Tier Compatibility

### 1. **Removed Paid Features**
- ❌ **`merge_group`**: Removed merge queue functionality (GitHub Teams/Enterprise feature)
- ❌ **`max-parallel`**: Removed concurrency limits (can cause billing on large repos)
- ❌ **CodeQL Advanced Security**: Disabled `.github/workflows/codeql.yml` (paid security feature)

### 2. **Reduced Matrix Testing**
- **Before**: 3 Python versions × 3 OS platforms = 9 combinations
- **After**: 1 Python version × 1 OS = 1 combination
- **Impact**: Faster CI, reduced compute usage, stays within free tier limits

### 3. **Optional Codecov Integration**
- **Condition Added**: Only uploads coverage if `CODECOV_TOKEN` secret exists
- **Benefit**: Won't fail if you don't have a Codecov account

### 4. **Component-Specific Testing**
- **New Workflow**: `.github/workflows/test-m5stack-8encoder.yml`
- **Purpose**: Quick testing of just the M5Stack 8Encoder component
- **Triggers**: Only runs when component files change

## Free Tier Limitations

GitHub Actions free tier includes:
- **2,000 minutes/month** for private repos
- **Unlimited minutes** for public repos  
- **500MB storage** for artifacts/caches

## Recommended Setup

1. **Keep repo public** for unlimited CI minutes
2. **Use component-specific workflow** for faster feedback
3. **Full CI runs** only on important branches (dev, main)
4. **Cache dependencies** to speed up builds

## Files Modified

```
.github/workflows/ci.yml           # Main CI - removed paid features
.github/workflows/codeql.yml       # Deleted - was paid feature
.github/workflows/codeql.yml.disabled # Backup of CodeQL workflow
.github/workflows/test-m5stack-8encoder.yml # New component-specific test
```

## Re-enabling Full Features

If you upgrade to GitHub Teams/Enterprise:

1. Restore `merge_group` in `ci.yml`
2. Re-add `max-parallel` limits for resource control  
3. Rename `codeql.yml.disabled` back to `codeql.yml`
4. Expand Python/OS matrix testing

## Component Testing

The component-specific workflow tests:
- ✅ **Configuration validation** - ensures YAML schemas work
- ✅ **ESP32 compilation** - verifies C++ code compiles for ESP32  
- ✅ **ESP8266 compilation** - verifies C++ code compiles for ESP8266
- ✅ **Python imports** - ensures component loads correctly

This provides good coverage for component development without excessive resource usage.