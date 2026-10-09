const { test } = require('node:test');
const assert = require('node:assert/strict');
const { nextVersion } = require('./release-version.cjs');

test('first release uses the baseline', () => {
    assert.equal(nextVersion('0.1.0', [], []), '0.1.0');
});

test('increments the highest numeric patch rather than sorting lexically', () => {
    assert.equal(nextVersion('0.1.0', ['mytools-v0.1.9', 'mytools-v0.1.10'], []), '0.1.11');
});

test('ignores unrelated and malformed tags', () => {
    assert.equal(nextVersion('0.1.0', ['v9.0.0', 'mytools-v01.1.0', 'mytools-v0.1.1-rc1'], []), '0.1.0');
});

test('rerunning the same merge reuses its tag', () => {
    assert.equal(nextVersion('0.1.0', ['mytools-v0.1.0'], ['mytools-v0.1.0']), '0.1.0');
});

test('rerunning an older release does not consume a new version', () => {
    assert.equal(
        nextVersion('0.1.0', ['mytools-v0.1.0', 'mytools-v0.1.1'], ['mytools-v0.1.0']),
        '0.1.0',
    );
});

test('raising the baseline starts a new minor or major series', () => {
    assert.equal(nextVersion('0.2.0', ['mytools-v0.1.9'], []), '0.2.0');
    assert.equal(nextVersion('1.0.0', ['mytools-v0.9.9'], []), '1.0.0');
});

test('does not reset the release version to an older baseline', () => {
    assert.equal(nextVersion('0.1.0', ['mytools-v1.2.3'], []), '1.2.4');
});

test('rejects invalid versions and overflow', () => {
    assert.throws(() => nextVersion('invalid', [], []), /Invalid release version/);
    assert.throws(() => nextVersion('0.1.0', ['mytools-v0.1.9007199254740991'], []), /safe integer/);
});
