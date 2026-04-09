// SPDX-FileCopyrightText: Copyright 2026 crueter
// SPDX-License-Identifier: GPL-3.0-or-later

import QtTest
import QDash.Items

/**
 * Qt Quick tests for DoubleSpinBox (the MLDoubleSpinBox drop-in).
 *
 * Every test creates a fresh DoubleSpinBox via createTemporaryObject so
 * that state cannot leak between test functions.  useLocaleFormat and
 * showGroupSeparator are disabled to keep formatting locale-independent.
 */
TestCase {
    name: "DoubleSpinBox"
    width: 240
    height: 80

    // Re-targeted by signal tests; cleared before each use.
    SignalSpy { id: valueModifiedSpy }

    Component {
        id: spinBoxComponent
        DoubleSpinBox {
            useLocaleFormat: false
            showGroupSeparator: false
        }
    }

    // -----------------------------------------------------------------------
    // Default property values
    // -----------------------------------------------------------------------

    function test_defaultValue() {
        var sb = createTemporaryObject(spinBoxComponent, testCase)
        compare(sb.value, 0.0)
    }

    function test_defaultFrom() {
        var sb = createTemporaryObject(spinBoxComponent, testCase)
        compare(sb.from, 0.0)
    }

    function test_defaultTo() {
        var sb = createTemporaryObject(spinBoxComponent, testCase)
        compare(sb.to, 100.0)
    }

    function test_defaultStepSize() {
        var sb = createTemporaryObject(spinBoxComponent, testCase)
        compare(sb.stepSize, 1.0)
    }

    function test_defaultDecimals() {
        var sb = createTemporaryObject(spinBoxComponent, testCase)
        compare(sb.decimals, 2)
    }

    function test_defaultWrapIsTrue() {
        var sb = createTemporaryObject(spinBoxComponent, testCase)
        verify(sb.wrap)
    }

    function test_defaultEditable() {
        var sb = createTemporaryObject(spinBoxComponent, testCase)
        verify(sb.editable)
    }

    // -----------------------------------------------------------------------
    // topValue / botValue derived properties
    // -----------------------------------------------------------------------

    function test_topValueWhenFromLessThanTo() {
        var sb = createTemporaryObject(spinBoxComponent, testCase, {from: 10.0, to: 90.0})
        compare(sb.topValue, 90.0)
    }

    function test_botValueWhenFromLessThanTo() {
        var sb = createTemporaryObject(spinBoxComponent, testCase, {from: 10.0, to: 90.0})
        compare(sb.botValue, 10.0)
    }

    function test_topValueWhenFromGreaterThanTo() {
        var sb = createTemporaryObject(spinBoxComponent, testCase, {from: 90.0, to: 10.0})
        compare(sb.topValue, 90.0)
    }

    function test_botValueWhenFromGreaterThanTo() {
        var sb = createTemporaryObject(spinBoxComponent, testCase, {from: 90.0, to: 10.0})
        compare(sb.botValue, 10.0)
    }

    // -----------------------------------------------------------------------
    // setValue() – return value
    // -----------------------------------------------------------------------

    function test_setValueReturnsTrueOnChange() {
        var sb = createTemporaryObject(spinBoxComponent, testCase, {from: -100.0, to: 100.0})
        verify(sb.setValue(42.0, true, true))
    }

    function test_setValueReturnsFalseWhenValueUnchanged() {
        var sb = createTemporaryObject(spinBoxComponent, testCase, {from: -100.0, to: 100.0})
        sb.setValue(42.0, true, true)
        verify(!sb.setValue(42.0, true, true))
    }

    // -----------------------------------------------------------------------
    // setValue() – clamping when wrap property is false
    // -----------------------------------------------------------------------

    function test_setValueClampsAboveTopWhenWrapIsFalse() {
        var sb = createTemporaryObject(spinBoxComponent, testCase, {wrap: false, from: 0.0, to: 100.0})
        sb.setValue(200.0)
        compare(sb.value, 100.0)
    }

    function test_setValueClampsBelowBotWhenWrapIsFalse() {
        var sb = createTemporaryObject(spinBoxComponent, testCase, {wrap: false, from: 0.0, to: 100.0})
        sb.setValue(-50.0)
        compare(sb.value, 0.0)
    }

    // -----------------------------------------------------------------------
    // setValue() – clamping via noWrap parameter (overrides wrap property)
    // -----------------------------------------------------------------------

    function test_setValueNoWrapParamClampsAboveTop() {
        var sb = createTemporaryObject(spinBoxComponent, testCase, {wrap: true, from: 0.0, to: 100.0})
        sb.setValue(200.0, true)   // noWrap = true
        compare(sb.value, 100.0)
    }

    function test_setValueNoWrapParamClampsBelowBot() {
        var sb = createTemporaryObject(spinBoxComponent, testCase, {wrap: true, from: 0.0, to: 100.0})
        sb.setValue(-10.0, true)   // noWrap = true
        compare(sb.value, 0.0)
    }

    // -----------------------------------------------------------------------
    // setValue() – wrapping (wrap = true, no noWrap override)
    // -----------------------------------------------------------------------

    function test_setValueWrapsToTopWhenBelowBot() {
        // value < botValue (0) → wraps to topValue (100)
        var sb = createTemporaryObject(spinBoxComponent, testCase, {wrap: true, from: 0.0, to: 100.0})
        sb.setValue(-5.0)
        compare(sb.value, 100.0)
    }

    function test_setValueWrapsToBottomWhenAboveTop() {
        // value > topValue (100) → wraps to botValue (0)
        var sb = createTemporaryObject(spinBoxComponent, testCase, {wrap: true, from: 0.0, to: 100.0})
        sb.setValue(150.0)
        compare(sb.value, 0.0)
    }

    // -----------------------------------------------------------------------
    // setValue() – rounding to decimals places
    // -----------------------------------------------------------------------

    function test_setValueRoundsToTwoDecimals() {
        var sb = createTemporaryObject(spinBoxComponent, testCase, {decimals: 2, from: -100.0, to: 100.0})
        sb.setValue(1.236, true, true)
        compare(sb.value, 1.24)
    }

    function test_setValueRoundsWithZeroDecimals() {
        var sb = createTemporaryObject(spinBoxComponent, testCase, {decimals: 0, from: -100.0, to: 100.0})
        sb.setValue(7.6, true, true)
        compare(sb.value, 8.0)
    }

    // -----------------------------------------------------------------------
    // setValue() – valueModified signal
    // -----------------------------------------------------------------------

    function test_setValueEmitsValueModified() {
        var sb = createTemporaryObject(spinBoxComponent, testCase, {from: -100.0, to: 100.0})
        valueModifiedSpy.target = sb
        valueModifiedSpy.signalName = "valueModified"
        valueModifiedSpy.clear()
        sb.setValue(42.0)
        compare(valueModifiedSpy.count, 1)
        valueModifiedSpy.target = null
    }

    function test_setValueDoesNotEmitValueModifiedWhenNotModifiedFlag() {
        var sb = createTemporaryObject(spinBoxComponent, testCase, {from: -100.0, to: 100.0})
        valueModifiedSpy.target = sb
        valueModifiedSpy.signalName = "valueModified"
        valueModifiedSpy.clear()
        sb.setValue(42.0, true, true)  // notModified = true
        compare(valueModifiedSpy.count, 0)
        valueModifiedSpy.target = null
    }

    // -----------------------------------------------------------------------
    // increase() / decrease()
    // -----------------------------------------------------------------------

    function test_increaseAddsOneStepFromDefault() {
        // value = 0 (default); textValue() = 0; after increase() → 0 + 5 = 5
        var sb = createTemporaryObject(spinBoxComponent, testCase,
                                       {from: 0.0, to: 100.0, stepSize: 5.0})
        sb.increase()
        compare(sb.value, 5.0)
    }

    function test_decreaseSubtractsOneStep() {
        // value = 0 (default); textValue() = 0; after decrease() → 0 - 5 = -5
        var sb = createTemporaryObject(spinBoxComponent, testCase,
                                       {wrap: false, from: -100.0, to: 100.0, stepSize: 5.0})
        sb.decrease()
        compare(sb.value, -5.0)
    }

    // -----------------------------------------------------------------------
    // stepBy()
    // -----------------------------------------------------------------------

    function test_stepByPositiveSteps() {
        var sb = createTemporaryObject(spinBoxComponent, testCase,
                                       {from: 0.0, to: 100.0, stepSize: 2.0})
        sb.stepBy(3)       // 0 + 3 × 2 = 6
        compare(sb.value, 6.0)
    }

    function test_stepByNegativeSteps() {
        var sb = createTemporaryObject(spinBoxComponent, testCase,
                                       {wrap: false, from: -100.0, to: 100.0, stepSize: 3.0})
        sb.stepBy(-2)      // 0 + (−2) × 3 = −6
        compare(sb.value, -6.0)
    }

    function test_stepByWithNoWrapParam() {
        // Even though wrap=true, the noWrap argument prevents wrapping.
        var sb = createTemporaryObject(spinBoxComponent, testCase,
                                       {wrap: true, from: 0.0, to: 50.0, stepSize: 10.0})
        // Start at top: clamp to 50, then try to go beyond with noWrap.
        sb.setValue(50.0, true, true)
        sb.stepBy(1, true)     // 50 + 10 = 60; noWrap → clamped to 50
        compare(sb.value, 50.0)
    }

    // -----------------------------------------------------------------------
    // textFromValue()
    // -----------------------------------------------------------------------

    function test_textFromValueIntegerWithTrimExtraZeros() {
        // 42.00 with trimExtraZeros=true → "42"
        var sb = createTemporaryObject(spinBoxComponent, testCase,
                                       {decimals: 2, trimExtraZeros: true})
        compare(sb.textFromValue(42.0, Qt.locale("C")), "42")
    }

    function test_textFromValueIntegerWithTrimExtraZerosFalse() {
        // 42.00 with trimExtraZeros=false → "42.00"
        var sb = createTemporaryObject(spinBoxComponent, testCase,
                                       {decimals: 2, trimExtraZeros: false})
        compare(sb.textFromValue(42.0, Qt.locale("C")), "42.00")
    }

    function test_textFromValueDecimalTrimmed() {
        // 1.5000 with trimExtraZeros=true → "1.5"
        var sb = createTemporaryObject(spinBoxComponent, testCase,
                                       {decimals: 4, trimExtraZeros: true})
        compare(sb.textFromValue(1.5, Qt.locale("C")), "1.5")
    }

    function test_textFromValueDecimalNotTrimmed() {
        // 1.5000 with trimExtraZeros=false → "1.5000"
        var sb = createTemporaryObject(spinBoxComponent, testCase,
                                       {decimals: 4, trimExtraZeros: false})
        compare(sb.textFromValue(1.5, Qt.locale("C")), "1.5000")
    }

    function test_textFromValueWithPrefix() {
        var sb = createTemporaryObject(spinBoxComponent, testCase,
                                       {prefix: "$", decimals: 2})
        var text = sb.textFromValue(42.0, Qt.locale("C"))
        verify(text.startsWith("$"), "Expected prefix '$', got: " + text)
    }

    function test_textFromValueWithSuffix() {
        var sb = createTemporaryObject(spinBoxComponent, testCase,
                                       {suffix: " m/s", decimals: 2})
        var text = sb.textFromValue(42.0, Qt.locale("C"))
        verify(text.endsWith(" m/s"), "Expected suffix ' m/s', got: " + text)
    }

    function test_textFromValueWithPrefixAndSuffix() {
        var sb = createTemporaryObject(spinBoxComponent, testCase,
                                       {prefix: "[", suffix: "]", decimals: 1, trimExtraZeros: false})
        var text = sb.textFromValue(5.0, Qt.locale("C"))
        verify(text.startsWith("["), "Expected '[' prefix, got: " + text)
        verify(text.endsWith("]"), "Expected ']' suffix, got: " + text)
    }

    // -----------------------------------------------------------------------
    // valueFromText()
    // -----------------------------------------------------------------------

    function test_valueFromTextPositive() {
        var sb = createTemporaryObject(spinBoxComponent, testCase)
        compare(sb.valueFromText("42.5", Qt.locale("C")), 42.5)
    }

    function test_valueFromTextNegative() {
        var sb = createTemporaryObject(spinBoxComponent, testCase)
        compare(sb.valueFromText("-10.75", Qt.locale("C")), -10.75)
    }

    function test_valueFromTextStripsPrefix() {
        var sb = createTemporaryObject(spinBoxComponent, testCase, {prefix: "$"})
        compare(sb.valueFromText("$42.5", Qt.locale("C")), 42.5)
    }

    function test_valueFromTextStripsSuffix() {
        var sb = createTemporaryObject(spinBoxComponent, testCase, {suffix: " kg"})
        compare(sb.valueFromText("42.5 kg", Qt.locale("C")), 42.5)
    }

    function test_textFromValueValueFromTextRoundTrip() {
        var sb = createTemporaryObject(spinBoxComponent, testCase,
                                       {decimals: 2, from: -1000.0, to: 1000.0})
        var original = 55.25
        var locale = Qt.locale("C")
        var text = sb.textFromValue(original, locale)
        compare(sb.valueFromText(text, locale), original)
    }

    // -----------------------------------------------------------------------
    // getCleanText()
    // -----------------------------------------------------------------------

    function test_getCleanTextTrimsWhitespace() {
        var sb = createTemporaryObject(spinBoxComponent, testCase)
        compare(sb.getCleanText("  42.5  "), "42.5")
    }

    function test_getCleanTextStripsPrefix() {
        var sb = createTemporaryObject(spinBoxComponent, testCase, {prefix: "val:"})
        compare(sb.getCleanText("val:42.5"), "42.5")
    }

    function test_getCleanTextStripsSuffix() {
        var sb = createTemporaryObject(spinBoxComponent, testCase, {suffix: " kg"})
        compare(sb.getCleanText("42.5 kg"), "42.5")
    }

    function test_getCleanTextNoAffixesPassesThrough() {
        var sb = createTemporaryObject(spinBoxComponent, testCase)
        compare(sb.getCleanText("100"), "100")
    }

    // -----------------------------------------------------------------------
    // escapeRegExpChars()
    // -----------------------------------------------------------------------

    function test_escapeRegExpCharsPlainText() {
        var sb = createTemporaryObject(spinBoxComponent, testCase)
        compare(sb.escapeRegExpChars("abc"), "abc")
    }

    function test_escapeRegExpCharsDot() {
        var sb = createTemporaryObject(spinBoxComponent, testCase)
        compare(sb.escapeRegExpChars("."), "\\.")
    }

    function test_escapeRegExpCharsPlus() {
        var sb = createTemporaryObject(spinBoxComponent, testCase)
        compare(sb.escapeRegExpChars("+"), "\\+")
    }

    function test_escapeRegExpCharsDollarSign() {
        var sb = createTemporaryObject(spinBoxComponent, testCase)
        compare(sb.escapeRegExpChars("$"), "\\$")
    }
}
