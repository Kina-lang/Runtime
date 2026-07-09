#!/usr/bin/env tsx

import { spawn } from "child_process";
import { existsSync } from "fs";
import { mkdir, readdir, rm, stat } from "fs/promises";
import path from "path";

const targetName = process.argv[2] ?? "x86_64-unknown-linux-gnu";

const isXtensa = targetName.startsWith("xtensa-");
const ccCommand = isXtensa ? `${targetName}-gcc` : "clang";
const arCommand = isXtensa ? `${targetName}-ar` : "ar";

console.log(`Building Kina Runtime for ${targetName}...`);
await main();
console.log("Done.");

async function collectFiles(dir: string) {
  const entries = await readdir(dir);
  const files: string[] = [];

  for (const entry of entries) {
    if ((await stat(path.join(dir, entry))).isDirectory()) {
      const subFiles = await collectFiles(path.join(dir, entry));
      files.push(...subFiles);

      continue;
    }
    if (!entry.endsWith(".c")) continue;

    files.push(path.join(dir, entry));
  }

  return files;
}

async function run(command: string, args: string[]) {
  return await new Promise<void>((res, rej) => {
    const proc = spawn(command, args, {
      stdio: "inherit",
      env: process.env,
      cwd: process.cwd(),
    });

    proc.on("exit", (code: number | null) => {
      if (code == null || code != 0)
        rej(`Process terminated with code ${code}`);
      else res();
    });
  });
}

async function buildFile(filePath: string, buildDir: string) {
  const fileName = path.basename(filePath, ".c");
  const outputFilePath = path.join(buildDir, `${fileName}.o`);

  const clangArgs = ["-c", filePath, "-o", outputFilePath];

  console.log(`Compiling ${filePath}...`);
  await run(ccCommand, clangArgs);
}

async function archiveFiles(buildDir: string, objDir: string) {
  const outputFilePath = path.join(buildDir, "kina-runtime.a");
  const objectFiles = (await readdir(objDir))
    .filter((file) => file.endsWith(".o"))
    .map((file) => path.join(objDir, file));

  const arArgs = ["rcs", outputFilePath, ...objectFiles];

  console.log(`Archiving object files into ${outputFilePath}...`);
  await run(arCommand, arArgs);
}

async function main() {
  const sourceDir = path.join(process.cwd(), "src");
  const buildDir = path.join(process.cwd(), "build", targetName);
  const objDir = path.join(buildDir, "obj");

  if (existsSync(buildDir))
    await rm(buildDir, { recursive: true, force: true });
  await mkdir(buildDir, { recursive: true });
  await mkdir(objDir, { recursive: true });

  const files = await collectFiles(sourceDir);

  for (const file of files) {
    await buildFile(file, objDir);
  }

  await archiveFiles(buildDir, objDir);
}
