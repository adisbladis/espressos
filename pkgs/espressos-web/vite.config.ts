import { Plugin, defineConfig } from "vite";
import solidPlugin from "vite-plugin-solid";
import { InputOptions } from "rollup";
import * as child from "child_process";

export default defineConfig({
  plugins: [
    solidPlugin(),

    // Build protobuf
    ((): Plugin => {
      return {
        name: "espressos-proto",
        buildStart: async () => {
          const p: child.ChildProcess = child.exec("npm run generate");
          await new Promise((resolve) => {
            p.stdout.on("data", (x) => {
              process.stdout.write(x.toString());
            });
            p.stderr.on("data", (x) => {
              process.stderr.write(x.toString());
            });
            p.on("exit", (code) => {
              resolve(code);
            });
          });
        },
      };
    })(),
  ],
  server: {
    port: 3000,
    proxy: {
      "/api": {
        target: "ws://10.1.1.137",
        ws: true,
      },
    },
  },
  build: {
    target: "esnext",
  },
});
