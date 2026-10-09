const { execFileSync } = require('node:child_process');
const { appendFileSync, readFileSync } = require('node:fs');

const tagPrefix = 'mytools-v';
const versionPattern = /^(0|[1-9]\d*)\.(0|[1-9]\d*)\.(0|[1-9]\d*)$/;

function nextVersion(base, tags, headTags) {
    const parse = (version) => {
        if (!versionPattern.test(version)) {
            throw new Error(`Invalid release version: ${version}`);
        }
        const parts = version.split('.').map(Number);
        if (!parts.every(Number.isSafeInteger)) {
            throw new Error(`Release version exceeds safe integer range: ${version}`);
        }
        return parts;
    };
    const compare = (a, b) => a[0] - b[0] || a[1] - b[1] || a[2] - b[2];
    const baseline = parse(base);
    const versions = tags
        .filter((tag) => tag.startsWith(tagPrefix) && versionPattern.test(tag.slice(tagPrefix.length)))
        .map((tag) => ({ tag, parts: parse(tag.slice(tagPrefix.length)) }))
        .sort((a, b) => compare(b.parts, a.parts));
    const existing = versions.find(({ tag }) => headTags.includes(tag));
    if (existing) {
        return existing.tag.slice(tagPrefix.length);
    }
    if (!versions.length || compare(baseline, versions[0].parts) > 0) {
        return base;
    }
    const latest = [...versions[0].parts];
    latest[2] += 1;
    const version = latest.join('.');
    parse(version);
    return version;
}

if (require.main === module) {
    const cmake = readFileSync('CMakeLists.txt', 'utf8');
    const baseline = cmake.match(/^set\(MYTOOLS_VERSION "([^"]+)"/m);
    if (!baseline) {
        throw new Error('CMakeLists.txt must declare the MYTOOLS_VERSION default');
    }
    const gitTags = (...args) => execFileSync('git', ['tag', ...args], { encoding: 'utf8' })
        .trim().split(/\r?\n/).filter(Boolean);
    const version = nextVersion(
        baseline[1],
        gitTags('--list', 'mytools-v*'),
        gitTags('--points-at', 'HEAD', '--list', 'mytools-v*'),
    );
    if (!process.env.GITHUB_OUTPUT) {
        throw new Error('GITHUB_OUTPUT is required');
    }
    appendFileSync(process.env.GITHUB_OUTPUT, `version=${version}\ntag=${tagPrefix}${version}\n`);
}

module.exports = { nextVersion };
