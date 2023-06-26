import { CapacitorConfig } from "@capacitor/cli";

const config: CapacitorConfig = {
  appId: "coffee.bar.espressos",
  appName: "espressos",
  webDir: "dist",
  server: {
    androidScheme: "http",
    cleartext: true,
  },
};

export default config;
